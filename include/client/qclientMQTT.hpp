#pragma once
/**
 *  https://github.com/iotaledger/tips/blob/main/tips/TIP-0028/tip-0028.md
 *
 **/


#include<QDebug>
#include <QMqttClient>
#include<QJsonValue>
#include "client/qwebsocketiodevice.hpp"
#include"block/qblock.hpp"

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
    ClientMqtt(QObject *parent = nullptr);

    ResponseMqtt* get_subscription(const QString& topic);
    ResponseMqtt* get_outputs_unlock_condition_address(const QString& condition_address);

    template<qblocks::Output::types outtype>
    ResponseMqtt* get_outputs_by_chain_id(const QString& id)
    {
        return get_subscription("outputs/"+qblocks::Output::typesstr[outtype]+"/"+id);
    }
    ResponseMqtt* get_outputs_outputId(const QString& outid);
    ResponseMqtt* get_blocks(void);
    void set_node_address(const QUrl &url){if(node_address_!=url&&url.isValid()){
            node_address_=url;emit node_address_changed();}}

signals:
    void node_address_changed();
private:
    void restart(void);
    void openDevice();
QUrl  node_address_;
quint16 port_;
WebSocketIODevice* m_device;
};

};
