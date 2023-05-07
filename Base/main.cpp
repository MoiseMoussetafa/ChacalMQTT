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

    for(int32_t s32_y = 0; s32_y < image.height(); ++s32_y)
    {
        for(int32_t s32_x = 0; s32_x < image.width(); ++s32_x)
        {
            QRgb pixel = image.pixel(s32_x, s32_y);

            int32_t s32_red = qRed(pixel);
            int32_t s32_alpha = qAlpha(pixel);

            if(s32_alpha == 0)
            {
                continue;
            }

            s_tmpString.append(QString::number(s32_red & 0x01));

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
    }

    return s_gpsCoordinates_decoded;
}


int32_t main(int32_t s32_argc, char_t *c_argv_tab[])
{
    QApplication a(s32_argc, c_argv_tab);
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

