#include "esp_ddns.h"

String api_get_global_ipv4_address = "https://4.ipw.cn";
String api_get_global_ipv6_address = "https://6.ipw.cn";
String api_cloudflare_v4 = "https://api.cloudflare.com/client/v4";

String get_global_ipv4_address(void)
{
    String global_ipv4_address = "";
    
    if (WiFi.status() == WL_CONNECTED)
    {
        HTTPClient http;
        
        http.begin(api_get_global_ipv4_address.c_str());

        uint16_t http_response_code = http.GET();
        String response = http.getString();

        http.end();

        Serial.print("HTTP Response Code: ");
        Serial.println(http_response_code);
        Serial.print("Response: ");
        Serial.println(response);

        global_ipv4_address = response;
    }
    else
    {
        Serial.println("Wifi Disconnected");
    }

    return global_ipv4_address;
}

IPv6Address get_global_ipv6_address(void)
{
    esp_ip6_addr_t ipv6_address;
    
    esp_netif_t* get_esp_interface_netif(esp_interface_t interface);
    
    if(esp_netif_get_ip6_global(get_esp_interface_netif(ESP_IF_WIFI_STA), &ipv6_address))
    {
        return IPv6Address();
    }
    
    return IPv6Address(ipv6_address.addr);
}

String get_root_domain(String sub_domain)
{
    String root_domain = "";
    
    std::smatch matches;
    std::string domain(sub_domain.c_str());
    std::regex regex_sub_domain(R"(^(?=^.{3,255}$)[a-zA-Z0-9][-a-zA-Z0-9]{0,61}[a-zA-Z0-9]\.([a-zA-Z0-9][-a-zA-Z0-9]{0,61}[a-zA-Z0-9]\.[a-zA-Z0-9][-a-zA-Z0-9]{0,61}[a-zA-Z0-9])$)");

    if (std::regex_search(domain, matches, regex_sub_domain))
    {
        root_domain = String(matches[1].str().c_str());
    }
    else
    {
        Serial.println("Invalid sub domain");
    }
    
    return root_domain;
}

String get_root_domain_id(String root_domain, String Authorization)
{
    String root_domain_id = "";
    
    if (WiFi.status() == WL_CONNECTED)
    {
        HTTPClient http;
        String api_cloudflare_get_root_domain_id = api_cloudflare_v4 + "/zones";
        
        http.begin(api_cloudflare_get_root_domain_id.c_str());
        
        http.addHeader("Content-Type", "application/json");
        http.addHeader("Authorization", Authorization);

        uint16_t http_response_code = http.GET();
        String response = http.getString();

        http.end();

        Serial.print("HTTP Response Code: ");
        Serial.println(http_response_code);
        Serial.print("Response: ");
        Serial.println(response);
        
        JsonDocument data;
        DeserializationError error = deserializeJson(data, response);

        if (error)
        {
            Serial.print("Error on JSON parsing: ");
            Serial.println(error.c_str());
        }
        else
        {
            if (data["success"])
            {
                JsonArray array = data["result"].as<JsonArray>();
                
                for (JsonObject obj : array)
                {
                    if (obj["name"].as<String>() == root_domain)
                    {
                        root_domain_id = obj["id"].as<String>();
                        break;
                    }
                }
            }
        }
    }
    else
    {
        Serial.println("Wifi Disconnected");
    }

    return root_domain_id;
}

String get_sub_domain_id(String sub_domain, String Authorization)
{
    String sub_domain_id = "";

    if (WiFi.status() == WL_CONNECTED)
    {
        HTTPClient http;
        String api_cloudflare_get_sub_domain_id = api_cloudflare_v4 + "/zones/" + get_root_domain_id(get_root_domain(sub_domain), Authorization) + "/dns_records";
        
        http.begin(api_cloudflare_get_sub_domain_id.c_str());
        
        http.addHeader("Content-Type", "application/json");
        http.addHeader("Authorization", Authorization);
        
        uint16_t http_response_code = http.GET();
        String response = http.getString();

        http.end();

        Serial.print("HTTP Response Code: ");
        Serial.println(http_response_code);
        Serial.print("Response: ");
        Serial.println(response);

        JsonDocument data;
        DeserializationError error = deserializeJson(data, response);

        if (error)
        {
            Serial.print("Error on JSON parsing: ");
            Serial.println(error.c_str());
        }
        else
        {
            if (data["success"])
            {
                JsonArray array = data["result"].as<JsonArray>();
                
                for (JsonObject obj : array)
                {
                    if (obj["name"].as<String>() == sub_domain)
                    {
                        sub_domain_id = obj["id"].as<String>();
                        break;
                    }
                }
            }
        }
    }
    else
    {
        Serial.println("Wifi Disconnected");
    }

    return sub_domain_id;
}

String get_dns_record_type(String ip_address)
{
    std::regex ipv4_regex(R"(^((\d|[1-9]\d|1\d\d|2[0-4]\d|25[0-5])\.){3}(\d|[1-9]\d|1\d\d|2[0-4]\d|25[0-5])$)");
    std::regex ipv6_regex(R"(^((([0-9A-Fa-f]{1,4}:){1,6}:)|(([0-9A-Fa-f]{1,4}:){7}))([0-9A-Fa-f]{1,4})$)");

    if (std::regex_match(ip_address.c_str(), ipv4_regex))
    {
        return "A";
    }
    else if (std::regex_match(ip_address.c_str(), ipv6_regex))
    {
        return "AAAA";
    }
    else
    {
        Serial.println("Invalid IP address");
        
        return "";
    }
}

void update_dns_record(String sub_domain, String Authorization, String ip_address)
{
    if (WiFi.status() == WL_CONNECTED)
    {
        HTTPClient http;
        String api_cloudflare_update_dns_record = api_cloudflare_v4 + "/zones/" + get_root_domain_id(get_root_domain(sub_domain), Authorization) + "/dns_records/" + get_sub_domain_id(sub_domain, Authorization);
        
        http.begin(api_cloudflare_update_dns_record.c_str());
        
        http.addHeader("Content-Type", "application/json");
        http.addHeader("Authorization", Authorization);
        
        String content = R"({"type":")" + get_dns_record_type(ip_address) + R"(","name":")" + sub_domain + R"(","ttl":1,"proxied":false,"content":")" + ip_address + R"("})";
        
        Serial.println(content);

        uint16_t http_response_code = http.PUT(content);
        String response = http.getString();

        http.end();

        Serial.print("HTTP Response Code: ");
        Serial.println(http_response_code);
        Serial.print("Response: ");
        Serial.println(response);
    }
    else
    {
        Serial.println("Wifi Disconnected");
    }
}
