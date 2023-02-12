#ifndef MQTT_H_
#define MQTT_H_

void mqtt_start();
void mqtt_send_message(char * topic, char * message);
void wifi_connected(void * params);

#endif /* MQTT_H_ */
