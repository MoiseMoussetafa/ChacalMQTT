#include <iostream>
#include <QCoreApplication>
#include <QImage>
#include <QDebug>
#include <QFile>
#include <QtMqtt/QMqttClient>
#include "send_png_on_topic.h"
#include "common.h"

using namespace std;

QString encodeCoordinates(QString &s_gpsCoordinates);

int32_t main(int32_t s32_argc, char_t *c_argv[])
{
    QCoreApplication a(s32_argc, c_argv);
    QString s_gpsCoordinates = QString("37°42'41.9\"S 144°59'33.0\"E");

    QImage image(":/images/img/DroneIMG37337.png");
    image = image.convertToFormat(QImage::Format_RGBA8888);

    if(image.isNull())
    {
        cout << "ERROR : Can't load image." << endl;
        return -1;
    }

    QString s_gpsBinary = encodeCoordinates(s_gpsCoordinates);

    for (int32_t int32_i = 0; int32_i < s_gpsBinary.size(); ++int32_i)
    {
        int32_t int32_x = int32_i % image.width();
        int32_t int32_y = int32_i / image.width();

        QRgb uint32_pixel = image.pixel(int32_x, int32_y);

        int32_t int32_alpha = qAlpha(uint32_pixel);
        int32_t int32_red = qRed(uint32_pixel);

        if (int32_alpha == 0)
        {
            int32_alpha = 1;
        }

        if (int32_i < image.width() * image.height())
        {
            int32_t int32_bit = s_gpsBinary[int32_i].digitValue();
            int32_red = (int32_red & ~1) | int32_bit;
        }

        image.setPixel(int32_x, int32_y, qRgba(int32_red, qGreen(uint32_pixel), qBlue(uint32_pixel), int32_alpha));
    }

    QFile qf_outputFile("DroneIMG37337_with_gps.png");
    if (!qf_outputFile.open(QIODevice::WriteOnly))
    {
        cerr << "ERROR : Can't save image." << endl;
        return -1;
    }
    
    image.save(&qf_outputFile, "PNG");

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

        const QString s_filePath("../build-drone-Desktop_Qt_5_12_12_GCC_64bit-Debug/DroneIMG37337_with_gps.png");
        bool success = send_png_on_topic(client, s_filePath, s_topic, qos_var);

        if (!success) {
            qDebug() << "La publication a échoué";
            return -1;
        }
    });

    client.connectToHost();
    return a.exec();
}

QString encodeCoordinates(QString &s_gpsCoordinates)
{
    QString s_gpsBinary;
    for (int32_t int32_i = 0; int32_i < s_gpsCoordinates.size(); ++int32_i)
    {
        QChar c_thisChar = s_gpsCoordinates[int32_i];

        s_gpsBinary.append(QString("%1").arg(c_thisChar.unicode(), 8, 2, QChar('0')));
    }

    return s_gpsBinary;
}
