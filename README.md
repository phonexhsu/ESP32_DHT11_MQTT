

# ESP32_DHT11_MQTT

 ESP32讀取DHT11溫濕度值後以MQTT回傳

## ESP32 - DHT11 接線

<div>
<img src="https://dustinb.github.io/images/NodeMCU-32S.png" />
</div>

<div>
<img src="https://ix23.com/wp-content/uploads/2018/02/DHT11-Pinout-for-Three-Pin-and-Four-Pin-Types-004a2-600x411.jpg" />
</div>

<div>
<img src="https://github.com/phonexhsu/ESP32_DHT11_MQTT/blob/main/pics/ESP32-DHT11_wiring.png?raw=true" />
</div>

在本專案中使用的是三個接腳的DHT11感測器。arduino project預設使用GPIO 4作為I2C bus。請將ESP32與DHT11依照上圖連接。
