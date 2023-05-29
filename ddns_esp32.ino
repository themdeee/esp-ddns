#include "ddns_esp32.h"

#define USE_IPV6    true

char ssid[] = "******";
char pass[] = "******";

void setup()
{
    Serial.begin(115200);

    WiFi.begin(ssid, pass);
    Serial.println("Connecting to WiFi ..");
    
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print('.');
        delay(1000);
    }

    #if USE_IPV6
    WiFi.enableIpV6();
    delay(3000);
    #endif

    Serial.println("Connected to the WiFi network");
    Serial.println(WiFi.localIP());

    #if USE_IPV6
    Serial.println(WiFi.localIPv6());
    #endif
}

void loop()
{
    #if !USE_IPV6
    update_dns_record(name, get_global_ipv4_address(), 4);
    #endif

    #if USE_IPV6
    update_dns_record(name, get_global_ipv6_address().toString(), 6);
    #endif

    delay(10000);
}
