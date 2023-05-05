#include "send_png_on_topic.h"
#include <QFile>

bool send_png_on_topic(QMqttClient &client, const QString &filePath, const QString &topic, const quint8 qos)
{
    // Chargez le fichier PNG
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Impossible de charger le fichier PNG";
        return false;
    }

    QByteArray data = file.readAll();
    //qDebug() << data;
    // Publiez le fichier sur le topic avec le QoS spécifié
    client.publish(topic, data, qos);
    qDebug() <<  "send";
    return true;
}
