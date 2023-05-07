#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QLabel>
#include <QtMqtt/QMqttClient>
#include <QDebug>
#include <QPixmap>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

signals:
    void signalImage(QImage);
    void signalCode(QString);

private slots:
    void processReceivedImage(QImage);
    void processCode(QString);

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QLabel *imageLabel;
    QPixmap pixmap_image;
    QImage image;

};
#endif // MAINWINDOW_H
