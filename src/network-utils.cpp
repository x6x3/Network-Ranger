#include "network-utils.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <unistd.h>
#include <netdb.h>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <regex>
#include <vector>
#include <map>

std::vector<NetworkInterface> NetworkUtils::getNetworkInterfaces() {
    std::vector<NetworkInterface> interfaces;
    
    struct ifaddrs* ifaddr;
    if (getifaddrs(&ifaddr) == -1) {
        return interfaces;
    }
    
    for (struct ifaddrs* ifa = ifaddr; ifa != nullptr; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == nullptr) continue;
        
        NetworkInterface interface;
        interface.name = ifa->ifa_name;
        
        if (ifa->ifa_addr->sa_family == AF_INET) {
            char ip[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &(((struct sockaddr_in*)ifa->ifa_addr)->sin_addr), ip, INET_ADDRSTRLEN);
            interface.ip_address = ip;
            interface.status = "UP";
            interface.speed = 1000;
            interfaces.push_back(interface);
        }
    }
    
    freeifaddrs(ifaddr);
    return interfaces;
}

std::string NetworkUtils::getCurrentSSID() {
    std::string ssid = "Unknown";
    
    FILE* pipe = popen("networksetup -getairportnetwork en0 2>/dev/null", "r");
    if (pipe) {
        char buffer[128];
        if (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
            std::string result = buffer;
            size_t pos = result.find(": ");
            if (pos != std::string::npos) {
                ssid = result.substr(pos + 2);
                ssid.erase(ssid.find_last_not_of(" \n\r\t") + 1);
            }
        }
        pclose(pipe);
    }
    
    return ssid;
}

int NetworkUtils::getSignalStrength() {
    int signal = -100;
    
    FILE* pipe = popen("system_profiler SPAirPortDataType 2>/dev/null | grep -A 5 'Current Network Information' | grep 'Signal / Noise'", "r");
    if (pipe) {
        char buffer[128];
        if (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
            std::string result = buffer;
            std::regex signal_regex(R"(\d+/\d+)");
            std::smatch match;
            if (std::regex_search(result, match, signal_regex)) {
                std::string signal_str = match.str();
                size_t slash_pos = signal_str.find('/');
                if (slash_pos != std::string::npos) {
                    signal = std::stoi(signal_str.substr(0, slash_pos));
                }
            }
        }
        pclose(pipe);
    }
    
    return signal;
}

std::string NetworkUtils::getGatewayIP() {
    std::string gateway = "Unknown";
    
    FILE* pipe = popen("netstat -nr | grep default | grep -v 'tun\\|tap' | awk '{print $2}' | head -1", "r");
    if (pipe) {
        char buffer[128];
        if (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
            gateway = buffer;
            gateway.erase(gateway.find_last_not_of(" \n\r\t") + 1);
        }
        pclose(pipe);
    }
    
    return gateway;
}

std::vector<std::string> NetworkUtils::getDNSServers() {
    std::vector<std::string> dns_servers;
    
    FILE* pipe = popen("cat /etc/resolv.conf | grep nameserver | awk '{print $2}'", "r");
    if (pipe) {
        char buffer[128];
        while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
            std::string dns = buffer;
            dns.erase(dns.find_last_not_of(" \n\r\t") + 1);
            if (!dns.empty()) {
                dns_servers.push_back(dns);
            }
        }
        pclose(pipe);
    }
    
    return dns_servers;
}

bool NetworkUtils::setDNSServers(const std::vector<std::string>& servers) {
    std::cout << "Setting DNS servers..." << std::endl;
    for (const auto& server : servers) {
        std::cout << "  DNS: " << server << std::endl;
    }
    
    std::string command = "networksetup -setdnsservers Wi-Fi ";
    for (const auto& server : servers) {
        command += server + " ";
    }
    
    int result = system(command.c_str());
    return result == 0;
}

bool NetworkUtils::pingHost(const std::string& host, int count) {
    std::string command = "ping -c " + std::to_string(count) + " " + host + " > /dev/null 2>&1";
    return system(command.c_str()) == 0;
}

double NetworkUtils::measureLatency(const std::string& host) {
    auto start = std::chrono::high_resolution_clock::now();
    
    if (!pingHost(host, 1)) {
        return -1.0;
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    return duration.count();
}

std::string NetworkUtils::getPublicIP() {
    CURL* curl = curl_easy_init();
    std::string response;
    
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "https://api.ipify.org");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);
        
        CURLcode res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        
        if (res == CURLE_OK) {
            return response;
        }
    }
    
    return "Unknown";
}

bool NetworkUtils::isInternetConnected() {
    return pingHost("8.8.8.8", 1);
}

std::vector<std::string> NetworkUtils::scanWiFiNetworks() {
    std::vector<std::string> networks;
    
    FILE* pipe = popen("networksetup -listpreferredwirelessnetworks en0 2>/dev/null", "r");
    if (pipe) {
        char buffer[256];
        while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
            std::string network = buffer;
            network.erase(network.find_last_not_of(" \n\r\t") + 1);
            if (!network.empty() && network != "Preferred networks on en0:") {
                networks.push_back(network);
            }
        }
        pclose(pipe);
    }
    
    if (networks.empty()) {
        FILE* pipe2 = popen("airport -s 2>/dev/null | tail -n +2 | awk '{print $1}'", "r");
        if (pipe2) {
            char buffer[256];
            while (fgets(buffer, sizeof(buffer), pipe2) != nullptr) {
                std::string network = buffer;
                network.erase(network.find_last_not_of(" \n\r\t") + 1);
                if (!network.empty()) {
                    networks.push_back(network);
                }
            }
            pclose(pipe2);
        }
    }
    
    return networks;
}

std::map<std::string, std::string> NetworkUtils::getNetworkDetails() {
    std::map<std::string, std::string> details;
    
    FILE* pipe = popen("system_profiler SPAirPortDataType 2>/dev/null", "r");
    if (pipe) {
        char buffer[512];
        std::string current_section = "";
        while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
            std::string line = buffer;
            line.erase(line.find_last_not_of(" \n\r\t") + 1);
            
            if (line.find("Current Network Information:") != std::string::npos) {
                current_section = "current";
            } else if (line.find("Signal / Noise:") != std::string::npos && current_section == "current") {
                details["signal"] = line;
            } else if (line.find("Channel:") != std::string::npos && current_section == "current") {
                details["channel"] = line;
            } else if (line.find("Security:") != std::string::npos && current_section == "current") {
                details["security"] = line;
            }
        }
        pclose(pipe);
    }
    
    return details;
}

size_t NetworkUtils::writeCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
    userp->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string NetworkUtils::performHTTPRequest(const std::string& url, const std::map<std::string, std::string>& headers) {
    CURL* curl = curl_easy_init();
    std::string response;
    
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        
        struct curl_slist* header_list = nullptr;
        for (const auto& header : headers) {
            std::string header_line = header.first + ": " + header.second;
            header_list = curl_slist_append(header_list, header_line.c_str());
        }
        
        if (header_list) {
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header_list);
        }
        
        CURLcode res = curl_easy_perform(curl);
        
        if (header_list) {
            curl_slist_free_all(header_list);
        }
        
        curl_easy_cleanup(curl);
        
        if (res == CURLE_OK) {
            return response;
        }
    }
    
    return "";
}

Json::Value NetworkUtils::parseJSON(const std::string& json_string) {
    Json::Value root;
    Json::Reader reader;
    
    if (reader.parse(json_string, root)) {
        return root;
    }
    
    return Json::Value();
}

std::string NetworkUtils::getSpeedtestServers() {
    return performHTTPRequest("https://www.speedtest.net/api/js/servers");
}

bool NetworkUtils::optimizeTCP() {
    std::cout << "Optimizing TCP settings..." << std::endl;
    
    std::vector<std::string> commands = {
        "sudo sysctl -w net.core.rmem_max=16777216",
        "sudo sysctl -w net.core.wmem_max=16777216",
        "sudo sysctl -w net.ipv4.tcp_rmem='4096 87380 16777216'",
        "sudo sysctl -w net.ipv4.tcp_wmem='4096 65536 16777216'",
        "sudo sysctl -w net.ipv4.tcp_congestion_control=bbr"
    };
    
    for (const auto& cmd : commands) {
        system(cmd.c_str());
    }
    
    return true;
}

bool NetworkUtils::optimizeWiFi() {
    std::cout << "Optimizing WiFi settings..." << std::endl;
    
    std::vector<std::string> commands = {
        "networksetup -setairportpower en0 on",
        "networksetup -setairportnetwork en0 $(networksetup -getairportnetwork en0 | cut -d: -f2 | xargs) $(security find-generic-password -D 'AirPort network password' -a $(networksetup -getairportnetwork en0 | cut -d: -f2 | xargs) -w)"
    };
    
    for (const auto& cmd : commands) {
        system(cmd.c_str());
    }
    
    return true;
}

bool NetworkUtils::setNetworkPriority(const std::string& interface) {
    std::cout << "Setting network priority for: " << interface << std::endl;
    
    std::string command = "networksetup -setnetworkserviceenabled " + interface + " on";
    return system(command.c_str()) == 0;
}

size_t NetworkUtils::writeData(void* ptr, size_t size, size_t nmemb, void* userdata) {
    std::string* response = static_cast<std::string*>(userdata);
    response->append(static_cast<char*>(ptr), size * nmemb);
    return size * nmemb;
} 