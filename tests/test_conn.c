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
    printf("Hello, World!\n");
    return 0;
}