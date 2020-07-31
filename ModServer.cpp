#include "ModServer.hpp"

#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>

#include "JsonConfig.hpp"

namespace
{
    ESP8266WebServer server{};

    const String ROOT_HTML =
        "<html>"
        "<head><title>ESP Matrix</title></head>"
        "<body>OK</body>"
        "</html>";

    const String PAGE_NOT_FOUND_HTML =
        "<html>"
        "<head><title>Error</title></head>"
        "<body>404 Page Not Found</body>"
        "</html>";

    void handleRoot()
    {
        server.send(200, "text/html", ROOT_HTML);
    }

    void handleNotFound()
    {
        server.send(404, "text/html", PAGE_NOT_FOUND_HTML);
    }
} // namespace

void ModServer::setup()
{
    if (WiFi.status() == WL_CONNECTED)
    {
        setupDone = true;

        MDNS.begin(F("espmatrix"));
        MDNS.addService("_http", "_tcp", 80);

        server.on("/", handleRoot);
        server.onNotFound(handleNotFound);
        server.begin();
    }
}

void ModServer::loop()
{
    if (!setupDone)
    {
        setup();
    }

    MDNS.update();
    server.handleClient();
}
