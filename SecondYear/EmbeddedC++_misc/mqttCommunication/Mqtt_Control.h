//
// Created by mirap on 4/27/2024.
//

#ifndef EXERCISE4_MQTT_H
#include "IPStack.h"
#include "Countdown.h"
#include "MQTTClient.h"
#include <string>
#include "Initialize.h"

#define LED_1 20
#define LED_2 21
#define LED_3 22
#define BUTTON_PIN 9
#define DISCONNECT_BUTTON 7


class Mqtt_control{
public:
    Mqtt_control(IPStack &ipstack, MQTT::Client<IPStack, Countdown> &client, MQTTPacket_connectData &data, const std::string &broker, int port) :
    ipstack(ipstack), mqttClient(client), data(data), br(broker), port(port) {};
    bool connectToBroker();
    void subscribeToTopic(const std::string& topic);
    void publishMessage(const std::string &topic, const std::string &message);
private:
    IPStack &ipstack;
    MQTT::Client<IPStack, Countdown> &mqttClient;
    MQTTPacket_connectData &data;
    std::string br;
    int port;

};



#define EXERCISE4_MQTT_H

#endif //EXERCISE4_MQTT_H
