#include "mainwindow.h"
#include <QApplication>
#include <QtMqtt/QMqttClient>
#include <QtGui/QImage>
#include <QtCore/QBuffer>
#include "common.h"

QString decodeCoordinates(QImage &image)
{
    QString s_tmpString = QString("");
    QString s_gpsCoordinates_decoded;

    for(int32_t x = 0; x < image.width(); ++x)
    {
        QRgb pixel = image.pixel(x, 0);

        int32_t red = qRed(pixel);
        int32_t alpha = qAlpha(pixel);

        if(alpha == 0)
        {
            continue;
        }

        s_tmpString.append(QString::number(red & 0x01));

        if(s_tmpString.length() == 8)
        {
            QChar c_thisChar = QChar(s_tmpString.toInt(nullptr, 2));
            s_gpsCoordinates_decoded.append(c_thisChar);

            if(c_thisChar.toUpper() == 'W' || c_thisChar.toUpper() == 'E')
            {
                break;
            }
            s_tmpString.clear();
        }
    }

    return s_gpsCoordinates_decoded;
}


int32_t main(int32_t argc, char_t *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    QMqttClient mqttClient;
    mqttClient.setHostname("broker.emqx.io");
    mqttClient.setPort(1883);

    QObject::connect(&mqttClient, &QMqttClient::connected, [&]()
    {
        const QString s_topic("/ynov/bordeaux/ChacalMQTT");
        const quint8 qos_var = 2;
        mqttClient.subscribe(s_topic,qos_var);
    });

    QObject::connect(&mqttClient, &QMqttClient::disconnected, [&]()
    {
    });

    QObject::connect(&mqttClient, &QMqttClient::messageReceived, [&](const QByteArray& message_tab, const QMqttTopicName& topic)
    {
        QImage image;
        image.loadFromData(message_tab, "PNG");
        QString s_gpsCoordinates_decoded = decodeCoordinates(image);
        emit w.signalCode(s_gpsCoordinates_decoded);
        emit w.signalImage(image);
        QString s_fileName = QString("../received_image.png");
        image.save(s_fileName);
    });

    mqttClient.connectToHost();
    w.show();
    return a.exec();
}

