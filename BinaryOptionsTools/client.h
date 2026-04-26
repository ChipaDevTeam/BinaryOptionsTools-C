#ifndef CLIENT_H
#define CLIENT_H
#include <stdio.h>
#include <string.h>
#include <libwebsockets.h>

#define TARGET_HOST    "demo-api-eu.po.market"
#define TARGET_PATH    "/socket.io/?EIO=4&transport=websocket"
#define TARGET_PORT    443
#define ORIGIN         "https://pocketoption.com"
#define USER_AGENT     "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/

static int callback(struct lws *wsi, enum lws_callback_reasons reason,
                    void *user, void *in, size_t len) {
    return 0;
}

static int interrupted = 0;


#endif // CLIENT_H