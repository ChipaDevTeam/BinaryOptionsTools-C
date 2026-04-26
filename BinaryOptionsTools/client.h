#ifndef CLIENT_H
#define CLIENT_H
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

typedef struct {
    struct lws_context *context;
    struct lws *wsi;
} WsClient;

WsClient WsClient_new();

#endif // CLIENT_H