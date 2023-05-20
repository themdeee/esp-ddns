#include "ddns_esp32.h"

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

    Serial.println("Connected to the WiFi network");
    Serial.println(WiFi.localIP());
}

void loop()
{
    update_dns_record(name, get_global_ipv4_address(), 4);

    delay(10000);
}
