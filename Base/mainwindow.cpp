#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtMqtt/QMqttClient>
#include <QDebug>
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Affichage des informations dans les textEdit
    ui->textEdit1->setPlainText("Broker : mqtttest://broker.emqx.io:1883");
    ui->textEdit2->setPlainText("Topic : /ynov/bordeaux/ChacalMQTT");
    ui->textEdit3->setPlainText("QoS 2");
    ui->textEdit4->setPlainText("Message décodé = ");

    // Affichage de l'image
    QPixmap image(":/prefix_img/DroneIMG37337.png");
    ui->imageLabel->setPixmap(image);
    ui->imageLabel->setScaledContents(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}
