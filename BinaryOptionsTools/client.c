#include <stdio.h>
#include <string.h>
#include <libwebsockets.h>

#include "client.h"

void WsClient() {
    struct lws_context_creation_info info;
    memset(&info, 0, sizeof(info));
    info.port = CONTEXT_PORT_NO_LISTEN;
    info.protocols = (struct lws_protocols[]) {
        { "example-protocol", callback, 0, 0 },
        { NULL, NULL, 0, 0 }
    };

    struct lws_context *context = lws_create_context(&info);
    if (!context) {
        fprintf(stderr, "Failed to create context\n");
        return;
    }

    struct lws_client_connect_info ccinfo = {0};
    ccinfo.context = context;
    ccinfo.address = TARGET_HOST;
    ccinfo.port = TARGET_PORT;
    ccinfo.path = TARGET_PATH;
    ccinfo.host = TARGET_HOST;
    ccinfo.origin = ORIGIN;
    ccinfo.protocol = "example-protocol";
    ccinfo.ssl_connection = 0;

    struct lws *wsi = lws_client_connect_via_info(&ccinfo);
    if (!wsi) {
        fprintf(stderr, "Failed to connect\n");
        lws_context_destroy(context);
        return;
    }

    while (!interrupted) {
        lws_service(context, 100);
    }

    lws_context_destroy(context);
}