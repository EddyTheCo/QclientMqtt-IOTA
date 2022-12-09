#pragma once
/**
 *  https://github.com/iotaledger/tips/blob/main/tips/TIP-0028/tip-0028.md
 *
 **/

#include<QByteArray>
#include<QDebug>
#include <QMqttClient>
#include<QJsonValue>
#include "client/qwebsocketiodevice.hpp"
namespace qiota{

class ResponseMqtt: public QObject
{
    Q_OBJECT
public:
    ResponseMqtt(QMqttSubscription * thesub);
public slots:
    void fill(QMqttMessage msg);
signals:
    void returned( QJsonValue data );
private:
    QMqttSubscription * sub_;

};


class ClientMqtt: public QMqttClient
{

public:
    ClientMqtt(const QUrl &url);

    ResponseMqtt*  get_subscription(const QString& topic);
    ResponseMqtt* get_outputs_unlock_condition_address(const QString& condition_address);
    ResponseMqtt* get_blocks(void);

private:
QUrl  node_address_;
quint16 port_;
WebSocketIODevice m_device;
};

};
