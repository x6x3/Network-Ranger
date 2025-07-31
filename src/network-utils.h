#ifndef NETWORK_UTILS_H
#define NETWORK_UTILS_H

#include <string>
#include <vector>
#include <map>
#include <curl/curl.h>
#include <json/json.h>

struct NetworkInterface {
    std::string name;
    std::string ip_address;
    std::string mac_address;
    std::string status;
    int speed;
};

struct SpeedTestConfig {
    std::string server_url;
    std::string api_key;
    int timeout;
    bool use_ssl;
};

class NetworkUtils {
public:
    static std::vector<NetworkInterface> getNetworkInterfaces();
    static std::string getCurrentSSID();
    static int getSignalStrength();
    static std::string getGatewayIP();
    static std::vector<std::string> getDNSServers();
    static bool setDNSServers(const std::vector<std::string>& servers);
    static bool pingHost(const std::string& host, int count = 4);
    static double measureLatency(const std::string& host);
    static std::string getPublicIP();
    static bool isInternetConnected();
    static size_t writeCallback(void* contents, size_t size, size_t nmemb, std::string* userp);
    static std::string performHTTPRequest(const std::string& url, const std::map<std::string, std::string>& headers = {});
    static Json::Value parseJSON(const std::string& json_string);
    static std::string getSpeedtestServers();
    static bool optimizeTCP();
    static bool optimizeWiFi();
    static bool setNetworkPriority(const std::string& interface);
    static std::vector<std::string> scanWiFiNetworks();
    static std::map<std::string, std::string> getNetworkDetails();
    
private:
    static size_t writeData(void* ptr, size_t size, size_t nmemb, void* userdata);
};

#endif 