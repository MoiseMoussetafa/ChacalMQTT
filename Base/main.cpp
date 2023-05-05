#include "mainwindow.h"
#include <QtMqtt/QMqttClient>
#include <QApplication>
#include <QtMqtt/QMqttClient>
#include <QtGui/QImage>
#include <QtCore/QBuffer>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    QMqttClient mqttClient;
    mqttClient.setHostname("broker.emqx.io");
    mqttClient.setPort(1883);

    QObject::connect(&mqttClient, &QMqttClient::connected, [&](){
        qDebug() << "Connected!";
        const QString s_topic("/ynov/bordeaux/ChacalMQTT");
        const quint8 qos_var = 2;
        mqttClient.subscribe(s_topic,qos_var);
    });

    QObject::connect(&mqttClient, &QMqttClient::disconnected, [&](){
        qDebug() << "Disconnected!";
    });

    QObject::connect(&mqttClient, &QMqttClient::messageReceived, [&](const QByteArray& message, const QMqttTopicName& topic){
        qDebug() << "Message received on topic:" << topic.name() << "size:" << message.size();

        QImage image;
        image.loadFromData(message, "PNG");
        qDebug() << image;
        QString fileName = QString("received_image.png");
        image.save(fileName);
    });

    mqttClient.connectToHost();

    w.show();
    return a.exec();
}
