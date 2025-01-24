# esp-ddns

- [esp-ddns介绍](#esp-ddns介绍)
- [有待实现](#有待实现)



## esp-ddns介绍

esp-ddns由[bt66](https://github.com/bt66)/[esp32-Cloudflare-DDNS](https://github.com/bt66/esp32-Cloudflare-DDNS)所启发，是一个利用[Cloudflare API](https://developers.cloudflare.com/api)，将esp32的公网ip，更新到托管在Cloudflare的域名的DNS解析记录的一个Arduino库。

同时感谢[XIU2](https://github.com/XIU2)/[CloudflareSpeedTest](https://github.com/XIU2/CloudflareSpeedTest/issues/40)，从中自己学到了很多东西。



## 有待实现

- [x] 使用[ArduinoJson库](https://github.com/bblanchon/ArduinoJson)解析Cloudflare API返回的数据

  - 获取根域名ID

  - 获取子域名解析记录ID
- [x] 支持ipv6

- [ ] 若`get_sub_domain_id`没有获取到对应的`sub_domain_id`，则创建一个对应的`sub_domain`
- [ ] 支持esp8266

