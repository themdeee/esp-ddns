#include "ddns_esp32.h"

#if USE_BEARER_AUTH
// use Bearer Auth
String Authorization = "******";
#endif

#if !USE_BEARER_AUTH
// use Global API Key
String X_Auth_Email = "******";
String X_Auth_Key = "******";
#endif

// 顶级域名
String zone_name = "******";
// 顶级域名ID
String zone_id = "******";
// 完整子域名
String name = "******";
// 解析记录ID
String id = "******";

// 获取ipv4公网地址网页
String web_get_global_ipv4_address = "https://4.ipw.cn";
// 获取ipv6公网地址网页
String web_get_global_ipv6_address = "https://6.ipw.cn";

String get_global_ipv4_address()
{
    HTTPClient http;
    String global_ipv4_address = "";
    
    if (WiFi.status() == WL_CONNECTED)
    {
        http.begin(web_get_global_ipv4_address.c_str());

        if (http.GET() == 200)
        {
            Serial.print("HTTP Response code: ");
            Serial.println(http.GET());

            global_ipv4_address = http.getString();
            Serial.println(global_ipv4_address);
        }
        else
        {
            Serial.print("Error code : ");
            Serial.println(http.GET());
        }
        
        http.end();
    }
    else
    {
        Serial.println("Wifi Disconnected");
    }

    return global_ipv4_address;
}

// 以后也许会有用
// String get_global_ipv6_address()
// {
//     HTTPClient http;
//     String global_ipv6_address = "";
    
//     if (WiFi.status() == WL_CONNECTED)
//     {
//         http.begin(web_get_global_ipv6_address.c_str());

//         if (http.GET() == 200)
//         {
//             Serial.print("HTTP Response code: ");
//             Serial.println(http.GET());

//             global_ipv6_address = http.getString();
//             Serial.println(global_ipv6_address);
//         }
//         else
//         {
//             Serial.print("Error code : ");
//             Serial.println(http.GET());
//         }
        
//         http.end();
//     }
//     else
//     {
//         Serial.println("Wifi Disconnected");
//     }

//     return global_ipv6_address;
// }

IPv6Address get_global_ipv6_address()
{
    esp_ip6_addr_t ipv6_address;
    
    esp_netif_t* get_esp_interface_netif(esp_interface_t interface);
    
    if(esp_netif_get_ip6_global(get_esp_interface_netif(ESP_IF_WIFI_STA), &ipv6_address))
    {
        return IPv6Address();
    }
    
    return IPv6Address(ipv6_address.addr);
}

void update_dns_record(String name, String ip_address, uint8_t version)
{
    HTTPClient http;
    String cloudflare_update_dns_record_api = "https://api.cloudflare.com/client/v4/zones/" + zone_id + "/dns_records/" + id;
    String content = "";
    
    if (WiFi.status() == WL_CONNECTED)
    {
        http.begin(cloudflare_update_dns_record_api.c_str());
        
        http.addHeader("Content-Type", "application/json");

        #if USE_BEARER_AUTH
        http.addHeader("Authorization", Authorization);
        #endif
        
        #if !USE_BEARER_AUTH
        http.addHeader("X-Auth-Email", X_Auth_Email);
        http.addHeader("X-Auth-Key", X_Auth_Key);
        #endif
        
        if (version == 4)
        {
            content = "{\"type\":\"A\",\"name\":\"" + name + "\",\"ttl\":1,\"proxied\":false,\"content\":\"" + ip_address + "\"}";
            Serial.println(content);
        }
        if (version == 6)
        {
            content = "{\"type\":\"AAAA\",\"name\":\"" + name + "\",\"ttl\":1,\"proxied\":false,\"content\":\"" + ip_address + "\"}";
            Serial.println(content);
        }
        
        if (http.PUT(content) == 200)
        {
            Serial.print("Cloudflare HTTP Response Code: ");
            Serial.println(http.PUT(content));
            Serial.println(http.getString());
        }
        else
        {
            Serial.print("Change Cloudflare record Error code : ");
            Serial.println(http.PUT(content));
            Serial.println(http.getString());
        }

        http.end();
    }
    else
    {
        Serial.println("Wifi Disconnected");
    }
}
