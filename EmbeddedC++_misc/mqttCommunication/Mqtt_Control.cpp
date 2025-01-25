#include "Mqtt_Control.h"
#include <iostream>
#include <map>


std::map<std::string, int> led_pins = {
        {"D1", LED_1},
        {"D2", LED_2},
        {"D3", LED_3}
};

//Fucntion to receive MQTT-messages
void messageArrived(MQTT::MessageData &md) {
    printf("Message received\n");
    MQTT::Message &message = md.message;
    char *payload = (char *) message.payload;
    int pin;
    const char *ptr = strstr(payload, "\"msg\":"); //find the position of message
    if (ptr != nullptr) {
        const char *msgptr = ptr + strlen("\"msg\":") + 1;
        ptr = strstr(msgptr, ";"); //find the semicolon
        if (ptr != nullptr){
            size_t len = ptr - msgptr;
            std::string led(msgptr, len);
            std::cout << "Led identifier: " << led << std::endl;
            pin = led_pins[led];
        }
        msgptr = strstr(payload, "\"}");
        ++ptr;
        if (msgptr != nullptr){
            size_t len = msgptr - ptr;
            std::string command(ptr, len);
            std::cout << "Led identifier: " << command << std::endl;
            if (command == "ON") gpio_put(pin, true);
            else if (command == "OFF") gpio_put(pin, false);
            else if (command == "TOGG") gpio_put(pin, !gpio_get(pin));
            else printf("Not working\n");
        }
    }
    else printf("Message not parsed correctly\n");
}


bool Mqtt_control::connectToBroker() {
    int rc = ipstack.connect(br.c_str(), port);
    if (rc != 0) {
        printf("rc from TCP connect is %d, stopping the program.\n", rc);
        return false;
    }
    printf("MQTT connecting\n");
    data.MQTTVersion = 3;
    data.clientID.cstring = (char *) "myHeadHurts"; 
    data.username.cstring = (char *) "SmartIoT";
    data.password.cstring = (char *) "SmartIoTMQTT"; 
    rc = mqttClient.connect(data);
    if (rc != 0) {
        printf("rc from MQTT CONNECT is %d\n", rc);
        return false;
    }
    printf("MQTT connected\n");
    return true;
}

//Subscribe to topic
void Mqtt_control::subscribeToTopic(const std::string& topic) {
    printf("Subscribing..\n");
    int rc = mqttClient.subscribe(topic.c_str(), MQTT::QOS0, messageArrived);
    if (rc != 0) {
        printf("rc from MQTT SUBSCRIBE is: %d", rc);
        return;
    }
    printf("MQTT subscribed to topic %s\n", topic.c_str());
}



void Mqtt_control::publishMessage(const std::string &topic, const std::string &message) {
    std::string json = "{\"topic\":\"" + topic + "\", \"msg\":\"" + message + "\"}";

    MQTT::Message mqttMessage;
    mqttMessage.retained = false;
    mqttMessage.dup = false;
    mqttMessage.qos = MQTT::QOS0;
    mqttMessage.payload = (void*) json.data();  // c_str() adds zero, data() does not
    mqttMessage.payloadlen = json.length();

    int rc = mqttClient.publish(topic.c_str(), mqttMessage);
    std::cout << "Published to topic " << topic << " message " << message << std::endl;
    std::cout << "Publish rc=" << rc << std::endl;
}
