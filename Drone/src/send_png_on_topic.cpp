#include "send_png_on_topic.h"
#include <QFile>
#include <iostream>

using namespace std;

bool send_png_on_topic(QMqttClient &mqttClient, const QString &s_filePath, const QString &topic, const quint8 qos)
{
    // Chargez le fichier PNG
    QFile qf_file(s_filePath);
    if (!qf_file.open(QIODevice::ReadOnly)) {
        cerr << "Impossible de charger le fichier PNG" << endl;
        return false;
    }

    QByteArray data = qf_file.readAll();

    // Publiez le fichier sur le topic avec le QoS spécifié
    mqttClient.publish(topic, data, qos);
    return true;
}
