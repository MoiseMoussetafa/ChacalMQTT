#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtMqtt/QMqttClient>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Affichage des informations dans les textEdit
    ui->textEdit1->setPlainText("Broker : mqtt://broker.emqx.io:1883");
    ui->textEdit2->setPlainText("Topic : /ynov/bordeaux/NomDuGitHubDuGroupe");
    ui->textEdit3->setPlainText("QoS 2");
    ui->textEdit4->setPlainText("Message décodé = ");
}

MainWindow::~MainWindow()
{
    delete ui;
}
