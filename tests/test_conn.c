#include <stdio.h>
#include <libwebsockets.h>

static int callback(struct lws *wsi, enum lws_callback_reasons reason,
                    void *user, void *in, size_t len) {
    switch (reason) {
        case LWS_CALLBACK_ESTABLISHED:
            printf("Connection established\n");
            break;
        case LWS_CALLBACK_RECEIVE:
            printf("Received data: %.*s\n", (int)len, (char *)in);
            break;
        case LWS_CALLBACK_CLOSED:
            printf("Connection closed\n");
            break;
        default:
            break;
    }
    return 0;
}

int main() {
    struct lws_context_creation_info info;
    struct lws_context *context;

    memset(&info, 0, sizeof(info));
    info.port = 8080;
    info.protocols = (struct lws_protocols[]) {
        { "http", callback, 0, 0 },
        { NULL, NULL, 0, 0 }
    };
    
    context = lws_create_context(&info);
    if (!context) {
        fprintf(stderr, "Failed to create context\n");
        return 1;
    }

    while (1) {
        lws_service(context, 1000);
    }

    lws_context_destroy(context);
    return 0;
}