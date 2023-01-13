#pragma once
/**
 *  https://github.com/iotaledger/tips/blob/main/tips/TIP-0028/tip-0028.md
 *
 **/


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
    void unsubscribe(void);
signals:
    void returned( QJsonValue data );
private:
    QMqttSubscription * sub_;

};


class ClientMqtt: public QMqttClient
{

    Q_OBJECT
public:
    ClientMqtt(const QUrl &url);

    ResponseMqtt*  get_subscription(const QString& topic);
    ResponseMqtt* get_outputs_unlock_condition_address(const QString& condition_address);
    ResponseMqtt* get_blocks(void);
    void set_node_address(const QUrl &url){if(node_address_!=url){node_address_=url;emit node_address_changed(node_address_);}}

signals:
    void node_address_changed(QUrl);
private:

    void openDevice(const QUrl& url);
QUrl  node_address_;
quint16 port_;
WebSocketIODevice m_device;
};

};
