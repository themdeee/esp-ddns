#ifndef __ESP_DDNS_H
#define __ESP_DDNS_H

#include <regex>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

String get_global_ipv4_address(void);
String get_root_domain(String sub_domain);
String get_root_domain_id(String root_domain, String Authorization);
String get_sub_domain_id(String sub_domain, String Authorization);
String get_dns_record_type(String ip_address);

#if ESP_ARDUINO_VERSION_MAJOR >= 3
    String get_global_ipv6_address(void);
#else
    IPv6Address get_global_ipv6_address(void);
#endif

void update_dns_record(String sub_domain, String Authorization, String ip_address);

#endif
