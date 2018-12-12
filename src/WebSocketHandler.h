#ifndef WEB_SOCKET_HANDLER_H
#define WEB_SOCKET_HANDLER_H

#include <WebSocketsServer.h>
#include "Settings.h"

class WebSocketHandler {
    public:
        WebSocketHandler(WebSocketsServer* webSocket, Settings& settings);
        ~WebSocketHandler() = default;
        void setup();
        void loop();
    private:
        WebSocketsServer* webSocket;
        Settings& settings;
        void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length);

};

#endif
