#include <iostream>
#include <QCoreApplication>
#include <QImage>
#include "common.h"
#include <QDebug>
#include <QFile>

using namespace std;

int32_t main(int32_t s32_argc, char_t *c_argv[])
{
    QCoreApplication a(s32_argc, c_argv);

    QImage image(":/images/img/DroneIMG37337.png");

    if(image.isNull())
    {
        cout << "ERROR : Can't load image" << endl;
        return -1;
    }

    QString gpsCoordinates = "37°42'41.9\"S 144°59'33.0\"E";
    QString gpsBinary;
    for (int32_t i = 0; i < gpsCoordinates.size(); ++i)
    {
        QChar c = gpsCoordinates[i];

        gpsBinary.append(QString("%1").arg(c.unicode(), 8, 2, QChar('0')));
    }

    qDebug() << gpsBinary << Qt::endl;

    for (int32_t i = 0; i < gpsBinary.size(); ++i)
    {
        int32_t x = i % image.width();
        int32_t y = i / image.width();
        qDebug() << "x: " << x << "| y : " << y << Qt::endl;

        QRgb pixel = image.pixel(x, y);

        int32_t alpha = qAlpha(pixel);
        int32_t red = qRed(pixel);
        int32_t green = qGreen(pixel);
        int32_t blue = qBlue(pixel);

        if (alpha == 0)
        {
            alpha = 1;
        }

        if (i < image.width() * image.height())
        {
            int32_t bit = gpsBinary[i].digitValue();
            red = (red & ~1) | bit;
        }

        qDebug() << pixel << Qt::endl;

        image.setPixel(x, y, qRgba(red, green, blue, alpha));
    }

    QFile outputFile("DroneIMG37337_with_gps.png");
    if (!outputFile.open(QIODevice::WriteOnly))
    {
        qDebug() << "Erreur : Impossible d'enregistrer l'image";
        return -1;
    }
    image.save(&outputFile, "PNG");

    return a.exec();
}
