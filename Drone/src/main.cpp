#include <iostream>
#include <QCoreApplication>
#include <QImage>
#include <QFile>
#include <QtMqtt/QMqttClient>
#include "send_png_on_topic.h"
#include "common.h"

using namespace std;

QString encodeCoordinates(const QString &s_gpsCoordinates);

int32_t main(int32_t s32_argc, char_t *c_argv_tab[])
{
    QCoreApplication a(s32_argc, c_argv_tab);
    const QString s_gpsCoordinates = QString("37°42'41.9\"S 144°59'33.0\"E");

    QImage image(":/images/img/DroneIMG37337.png");
    image = image.convertToFormat(QImage::Format_RGBA8888);

    if(image.isNull())
    {
        cout << "ERROR : Can't load image" << endl;
        return -1;
    }

    const QString s_gpsBinary = encodeCoordinates(s_gpsCoordinates);

    for (int32_t s32_i = 0; s32_i < s_gpsBinary.size(); ++s32_i)
    {
        int32_t s32_x = s32_i % image.width();
        int32_t s32_y = s32_i / image.width();

        QRgb pixel = image.pixel(s32_x, s32_y);

        int32_t s32_alpha = qAlpha(pixel);
        int32_t s32_red = qRed(pixel);

        if (s32_alpha == 0)
        {
            s32_alpha = 1;
        }

        if (s32_i < image.width() * image.height())
        {
            int32_t s32_bit = s_gpsBinary[s32_i].digitValue();
            s32_red = (s32_red & ~1) | s32_bit;
        }

        image.setPixel(s32_x, s32_y, qRgba(s32_red, qGreen(pixel), qBlue(pixel), s32_alpha));
    }

    QFile qf_outputFile("../DroneIMG37337_with_gps.png");
    if (!qf_outputFile.open(QIODevice::WriteOnly))
    {
        cerr << "Erreur : Impossible d'enregistrer l'image" << endl;
        return -1;
    }
    image.save(&qf_outputFile, "PNG");

    QMqttClient mqttClient;
    mqttClient.setHostname("broker.emqx.io");
    mqttClient.setPort(1883);

    QObject::connect(&mqttClient, &QMqttClient::connected, [&](void) {
        const QString s_topic("/ynov/bordeaux/ChacalMQTT");
        const quint8 qos_var = 2;
        const QString s_filePath("../DroneIMG37337_with_gps.png");
        send_png_on_topic(mqttClient, s_filePath, s_topic, qos_var);
    });

    mqttClient.connectToHost();
    return a.exec();
}

QString encodeCoordinates(const QString &s_gpsCoordinates)
{
    QString s_gpsBinary;
    for (int32_t s32_i = 0; s32_i < s_gpsCoordinates.size(); ++s32_i)
    {
        QChar c = s_gpsCoordinates[s32_i];

        s_gpsBinary.append(QString("%1").arg(c.unicode(), 8, 2, QChar('0')));
    }

    return s_gpsBinary;
}
