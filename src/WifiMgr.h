#ifndef WIFI_MGR_H
#define WIFI_MGR_H

#include "Settings.h"
#include "WebServer.h"

//#include <DNSServer.h>

//DNSServer dnsServer;

//const byte DNS_PORT = 53;

// bool apMode = false;


#define HOSTNAME "ESP8266-" ///< Hostname. The initializeWiFi function adds the Chip ID at the end.

#define DEBUG_WIFI 1




class WifiMgr {
    public:
        WifiMgr(Settings& settings, WebServer& webServer);

        // AP mode password
        const char WiFiAPPSK[] = {};

        // Wi-Fi network to connect to (if not in AP mode)
        char* ssid = "";
        char* password = "";

        unsigned long futureTimeout = 0;
        uint16_t connectionTimeout = 20000;


        template <typename Generic> void debugPrintln(Generic text);
        void startAp();
        String getWiFiJson();
        void checkWiFi();

        void initializeWiFi();
    private:
        Settings& settings;
        WebServer& webServer;
};

#endif