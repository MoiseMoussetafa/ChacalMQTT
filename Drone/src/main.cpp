#include <QCoreApplication>
#include <QtMqtt/QMqttClient>
#include "send_png_on_topic.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QMqttClient client;
    client.setHostname("broker.emqx.io");
    client.setPort(1883);

    QObject::connect(&client, &QMqttClient::connected, [&](void) {
        qDebug() << "Connected to MQTT broker.";
        const QString s_topic("/ynov/bordeaux/ChacalMQTT");
        const quint8 qos_var = 2;

        if (!client.subscribe(s_topic, qos_var)) {
            qDebug() << "Error while subscribing to :" << s_topic;
            return 1;
        } else {
            qDebug() << "Subscribed to topic:" << s_topic;
        }

        const QString s_filePath("../DroneIMG37337.png");
        bool success = send_png_on_topic(client, s_filePath, s_topic, qos_var);

        if (!success) {
            qDebug() << "La publication a échoué";
            return -1;
        }
    });

    client.connectToHost();
    return a.exec();
}
