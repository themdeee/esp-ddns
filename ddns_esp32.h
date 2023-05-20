#ifndef __DDNS_ESP32_H
#define __DDNS_ESP32_H

#include "HTTPClient.h"

#define USE_BEARER_AUTH     true

#if USE_BEARER_AUTH
extern String Authorization;
#endif

#if !USE_BEARER_AUTH
extern String X_Auth_Email;
extern String X_Auth_Key;
#endif

extern String zone_name;
extern String zone_id;
extern String name;
extern String id;
extern String web_get_global_ipv4_address;
extern String web_get_global_ipv6_address;

String get_global_ipv4_address();

void update_dns_record(String name, String ip_address, uint8_t version);

#endif
