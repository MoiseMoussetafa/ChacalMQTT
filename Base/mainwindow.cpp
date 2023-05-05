#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Affichage des informations dans les textEdit
    ui->textEdit1->setPlainText("Broker : mqtt://broker.emqx.io:1883");
    ui->textEdit2->setPlainText("Topic : /ynov/bordeaux/ChacalMQTT");
    ui->textEdit3->setPlainText("QoS 2");
    ui->textEdit4->setPlainText("Message décodé = ");

    // Affichage de l'image
    pixmap_image = QPixmap(":/prefix_img/build-Base_code-Desktop_Qt_5_15_2_GCC_64bit-Debug/received_image.png");
    ui->imageLabel->setPixmap(pixmap_image);
    ui->imageLabel->setScaledContents(true);
    connect(this, &MainWindow::signalImage, this, &MainWindow::processReceivedImage);
}

void MainWindow::processReceivedImage(QImage image)
{
    pixmap_image = QPixmap::fromImage(image);
    ui->imageLabel->setPixmap(pixmap_image);
    qDebug() << "New image";
}

MainWindow::~MainWindow()
{
    delete ui;
}
