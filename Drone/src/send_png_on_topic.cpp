#include "send_png_on_topic.h"
#include <QFile>
#include "common.h"

bool send_png_on_topic(QMqttClient &mqttClient, const QString &s_filePath, const QString &s_topic, const char c_qos)
{
    QFile qf_file(s_filePath);
    if (!qf_file.open(QIODevice::ReadOnly)) {
        return false;
    }
    QByteArray data_tab = qf_file.readAll();
    mqttClient.publish(s_topic, data_tab, c_qos);
    return true;
}
