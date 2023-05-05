#ifndef SEND_PNG_ON_TOPIC_H
#define SEND_PNG_ON_TOPIC_H

#include <QtMqtt/QMqttClient>

bool send_png_on_topic(QMqttClient &client, const QString &filePath, const QString &topic, const quint8 qos);

#endif // SEND_PNG_ON_TOPIC_H
