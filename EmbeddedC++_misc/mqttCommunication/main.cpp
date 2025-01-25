#include <iostream>
#include "pico/stdlib.h"
#include "pico/time.h"
#include "hardware/timer.h"
#include <queue>
#include <cstring>
#include "IPStack.h"
#include "Countdown.h"
#include "MQTTClient.h"
#include "Initialize.h"
#include "Mqtt_Control.h"
#include "wifi_config.h"

#define DB_INTERVAL 300000


int main() {
    stdio_init_all();
    std::cout << "Boot" << std::endl;
    Led(LED_1);
    Led(LED_2);
    Led(LED_3);
    Button(BUTTON_PIN);
    Button(DISCONNECT_BUTTON);

    //CREDENTIALS//
    std::string ssid = WIFI_SSID;
    std::string password = WIFI_PASSWORD;
    std::string broker = BROKER_IP;
    std::string clientID = "myHeadHurts";
    int port = 21883;

    std::string subTopic = "iAmUnique/LED";
    std::string pubTopic = "something";
    std::string msg = "Connected to broker.";

    IPStack ipstack(ssid.c_str(), password.c_str());
    auto client = MQTT::Client<IPStack, Countdown>(ipstack);
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;

    Mqtt_control mqtt_instance = Mqtt_control(ipstack, client, data, broker, port);

    if (!mqtt_instance.connectToBroker()){
        printf("Not connected to broker, bye bye.\n");
        return 0;
    }

    mqtt_instance.subscribeToTopic(subTopic);

    uint64_t current = 0;
    uint64_t last_pressed= 0;
    while (true) {
        if (!gpio_get(BUTTON_PIN)){
            current = time_us_64();
            if (current - last_pressed > DB_INTERVAL){
                last_pressed = current;
                mqtt_instance.publishMessage(pubTopic, msg);
            }
        }
        if (!gpio_get(DISCONNECT_BUTTON)){
            current = time_us_64();
            if (current - last_pressed > DB_INTERVAL){
                last_pressed = current;
                printf("Disconnecting\n");
                ipstack.disconnect();
                client.unsubscribe(subTopic.c_str());
                return 0;
            }
        }
        cyw43_arch_poll();
        client.yield(100); // socket that client uses calls cyw43_arch_poll()
    }
    return 0;
}