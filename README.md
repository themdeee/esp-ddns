# ddns_esp32

- [ddns_esp32介绍](#ddns_esp32介绍)
- [有待实现](#有待实现)



## ddns_esp32介绍

ddns_esp32由[bt66](https://github.com/bt66)/[esp32-Cloudflare-DDNS](https://github.com/bt66/esp32-Cloudflare-DDNS)所启发，是一个利用[Cloudflare API](https://developers.cloudflare.com/api)，将esp32的公网ip，更新到托管在Cloudflare的域名的DNS解析记录的一个Arduino库。

同时感谢[XIU2](https://github.com/XIU2)/[CloudflareSpeedTest](https://github.com/XIU2/CloudflareSpeedTest/issues/40)，从中自己学到了很多东西。



## 有待实现

使用[ArduinoJson库](https://github.com/bblanchon/ArduinoJson)解析Cloudflare API返回的数据

- [ ] 检验[API 令牌](https://dash.cloudflare.com/profile/api-tokens)可用性
- [ ] 获取域名ID
- [ ] 获取域名解析记录ID



- [ ] 支持ipv6

> 更新ipv6 DNS解析记录的功能理论上已经实现了，但`get_global_ipv6_address()`在目前官方的Arduino core for the ESP32（v2.0.9 20230520）下能编译，但运行会报错，或许在未来的v3.0.0中能解决这个问题吧
>
> 不过获取esp32的ipv6公网ip也可以通过[`WiFi.globalIPv6()`](https://github.com/espressif/arduino-esp32/pull/7065)来实现，❗注意❗这是第三方代码，并未合并到官方代码中

