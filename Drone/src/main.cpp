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
QString decodeCoordinates(QImage &image);

int32_t main(int32_t s32_argc, char_t *c_argv[])
{
    QCoreApplication a(s32_argc, c_argv);
    QString s_gpsCoordinates = QString("37°42'41.9\"S 144°59'33.0\"E");

    QImage image(":/images/img/DroneIMG37337.png");
    image = image.convertToFormat(QImage::Format_RGBA8888);

    if(image.isNull())
    {
        cout << "ERROR : Can't load image" << endl;
        return -1;
    }

    QString s_gpsBinary = encodeCoordinates(s_gpsCoordinates);

    for (int32_t i = 0; i < s_gpsBinary.size(); ++i)
    {
        int32_t x = i % image.width();
        int32_t y = i / image.width();

        QRgb pixel = image.pixel(x, y);

        int32_t alpha = qAlpha(pixel);
        int32_t red = qRed(pixel);

        if (alpha == 0)
        {
            alpha = 1;
        }

        if (i < image.width() * image.height())
        {
            int32_t bit = s_gpsBinary[i].digitValue();
            red = (red & ~1) | bit;
        }

        image.setPixel(x, y, qRgba(red, qGreen(pixel), qBlue(pixel), alpha));
    }

    QFile qf_outputFile("../DroneIMG37337_with_gps.png");
    if (!qf_outputFile.open(QIODevice::WriteOnly))
    {
        qDebug() << "Erreur : Impossible d'enregistrer l'image";
        return -1;
    }
    image.save(&outputFile, "PNG");

    QString gpsCoordinates_decoded = decodeCoordinates(image);

    qDebug() << gpsCoordinates_decoded;

    QMqttClient client;
    client.setHostname("broker.emqx.io");
    client.setPort(1883);

    QObject::connect(&client, &QMqttClient::connected, [&](void) {
        qDebug() << "Connected to MQTT broker.";
        const QString s_topic("/ynov/bordeaux/ChacalMQTT");
        const quint8 qos_var = 2;
        const QString s_filePath("../DroneIMG37337_with_gps.png");
        send_png_on_topic(mqttClient, s_filePath, s_topic, qos_var);
    });

    client.connectToHost();
    return a.exec();
}

QString encodeCoordinates(QString &s_gpsCoordinates)
{
    QString s_gpsBinary;
    for (int32_t i = 0; i < s_gpsCoordinates.size(); ++i)
    {
        QChar c = s_gpsCoordinates[i];

        s_gpsBinary.append(QString("%1").arg(c.unicode(), 8, 2, QChar('0')));
    }

    return s_gpsBinary;
}

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
