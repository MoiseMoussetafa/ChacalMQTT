#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->textEdit1->setPlainText("Broker : mqtt://broker.emqx.io:1883");
    ui->textEdit2->setPlainText("Topic : /ynov/bordeaux/ChacalMQTT");
    ui->textEdit3->setPlainText("QoS 2");
    ui->textEdit4->setPlainText("Message décodé = ");

    pixmap_image = QPixmap(":/prefix_img/received_image.png");
    ui->imageLabel->setPixmap(pixmap_image);
    ui->imageLabel->setScaledContents(true);
    connect(this, &MainWindow::signalImage, this, &MainWindow::processReceivedImage);

    connect(this, &MainWindow::signalCode, this, &MainWindow::processCode);
}
void MainWindow::processCode(QString gpsCoordinates_decoded)
{
    ui->textEdit4->setPlainText(gpsCoordinates_decoded);
}

void MainWindow::processReceivedImage(QImage image)
{
    pixmap_image = QPixmap::fromImage(image);
    ui->imageLabel->setPixmap(pixmap_image);
}

MainWindow::~MainWindow()
{
    delete ui;
}
