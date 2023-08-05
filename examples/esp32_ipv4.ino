#include "esp_ddns.h"

char ssid[] = "";
char pass[] = "";

String sub_domain = "xxx.yyy.zzz";
String Authorization = "Bearer xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";

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

    update_dns_record(sub_domain, Authorization, get_global_ipv4_address());
}

void loop()
{
    
}
