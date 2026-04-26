#include <stdio.h>
#include <string.h>
#include <libwebsockets.h>

#define TARGET_HOST    "demo-api-eu.po.market"
#define TARGET_PATH    "/socket.io/?EIO=4&transport=websocket"
#define TARGET_PORT    443
#define ORIGIN         "https://pocketoption.com"
#define USER_AGENT     "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/146.0.0.0 Safari/537.36 OPR/130.0.0.0 (Edition std-2)"

static int interrupted = 0;

static int callback(struct lws *wsi, enum lws_callback_reasons reason,
                    void *user, void *in, size_t len) {
    switch (reason) {
        case LWS_CALLBACK_CLIENT_ESTABLISHED:
            printf("Connection established\n");
            break;

        case LWS_CALLBACK_CLIENT_RECEIVE:
            printf("Received: %.*s\n", (int)len, (char *)in);
            break;

        case LWS_CALLBACK_CLIENT_CLOSED:
            printf("Connection closed\n");
            interrupted = 1;
            break;

        case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
            fprintf(stderr, "Connection error: %s\n", in ? (char *)in : "(null)");
            interrupted = 1;
            break;

        case LWS_CALLBACK_CLIENT_APPEND_HANDSHAKE_HEADER: {
            unsigned char **p = (unsigned char **)in;
            unsigned char *end = (*p) + len;

            if (lws_add_http_header_by_name(wsi,
                    (unsigned char *)"Cache-Control:",
                    (unsigned char *)"no-cache", 8, p, end)) return -1;

            if (lws_add_http_header_by_name(wsi,
                    (unsigned char *)"Accept-Language:",
                    (unsigned char *)"en-US,en;q=0.9", 14, p, end)) return -1;

            if (lws_add_http_header_by_name(wsi,
                    (unsigned char *)"Pragma:",
                    (unsigned char *)"no-cache", 8, p, end)) return -1;

            if (lws_add_http_header_by_name(wsi,
                    (unsigned char *)"User-Agent:",
                    (unsigned char *)USER_AGENT, strlen(USER_AGENT), p, end)) return -1;
            break;
        }

        default:
            break;
    }
    return 0;
}

static struct lws_protocols protocols[] = {
    { "ws", callback, 0, 65536, 0, NULL, 0 },
    LWS_PROTOCOL_LIST_TERM
};

int main() {
    struct lws_context_creation_info info;
    struct lws_client_connect_info ccinfo;
    struct lws_context *context;

    memset(&info, 0, sizeof(info));
    info.port = CONTEXT_PORT_NO_LISTEN;
    info.protocols = protocols;
    info.options = LWS_SERVER_OPTION_DO_SSL_GLOBAL_INIT;

    context = lws_create_context(&info);
    if (!context) {
        fprintf(stderr, "Failed to create context\n");
        return 1;
    }

    memset(&ccinfo, 0, sizeof(ccinfo));
    ccinfo.context  = context;
    ccinfo.address  = TARGET_HOST;
    ccinfo.port     = TARGET_PORT;
    ccinfo.path     = TARGET_PATH;
    ccinfo.host     = TARGET_HOST;
    ccinfo.origin   = ORIGIN;
    ccinfo.protocol = protocols[0].name;
    ccinfo.ssl_connection = LCCSCF_USE_SSL;

    if (!lws_client_connect_via_info(&ccinfo)) {
        fprintf(stderr, "Failed to initiate connection\n");
        lws_context_destroy(context);
        return 1;
    }

    while (!interrupted) {
        lws_service(context, 1000);
    }

    lws_context_destroy(context);
    return 0;
}