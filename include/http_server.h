#ifndef __HTTP_API_H__
#define __HTTP_API_H__

#include "ESPAsyncTCP.h"
#include "ESPAsyncWebServer.h"

typedef std::function<void(AsyncWebServerRequest*, std::function<void(AsyncWebServerRequest*)>)> middleware_t;

class HTTPServer {
    in_port_t port;

    AsyncWebServer *server = nullptr;
    middleware_t  middleware;

public:
    void initialize(in_port_t port = 80);

    void register_middleware(middleware_t middleware);
    void register_handler(const char *url, WebRequestMethod method, const std::function<void(AsyncWebServerRequest *)>& handler);

    AsyncWebServer *get_server();
};


#endif // __HTTP_API_H__
