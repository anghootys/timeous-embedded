#include "http_server.h"

#include <utility>
#include "core.h"
#include "logger.h"

#define LOG_HTTP_SERVER_CTX "HTTP"

LoggerFacade http_logger(Logger::setup(LOG_HTTP_SERVER_CTX));

void
HTTPServer::initialize(in_port_t port) {
    this->port = port;
    this->server = new AsyncWebServer(this->port);
}

AsyncWebServer *
HTTPServer::get_server() {
    if (this->server == nullptr) {
        http_logger.error("could not create establish HTTP Server.");
        reset_processor();
    }

    return this->server;
}
void
HTTPServer::register_middleware(middleware_t middleware) {
    this->middleware = std::move(middleware);
}

void
HTTPServer::register_handler(const char *url, WebRequestMethod method,
                             const std::function<void(AsyncWebServerRequest *)> &handler) {
    this->server->on(url, method,
                     [handler, this](AsyncWebServerRequest *request) { this->middleware(request, handler); });
}
