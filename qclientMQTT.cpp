#include"client/qclientMQTT.hpp"
#include<QJsonDocument>
#include<QJsonObject>
#include<QJsonValue>
#include<QTimer>
namespace qiota{

ResponseMqtt::ResponseMqtt(QMqttSubscription * thesub):sub_(thesub)
{
    QObject::connect(sub_,&QMqttSubscription::messageReceived ,this, &ResponseMqtt::fill);
}
void ResponseMqtt::fill(QMqttMessage msg)
{
    emit returned((QJsonDocument::fromJson(msg.payload())).object());
}
void ResponseMqtt::unsubscribe(void)
{
    sub_->unsubscribe();
    deleteLater();
}
void ClientMqtt::openDevice()
{
    m_device->setUrl(node_address_);
    if (!m_device->open(QIODevice::ReadWrite))
    {
        qDebug() << "Could not open socket device";
    }
    QTimer::singleShot(10000,this,[&](){if(!m_device->isOpen())this->restart();});

}
void ClientMqtt::restart(void)
{
    this->disconnectFromHost();
    if(m_device->isOpen())m_device->close();
    openDevice();

}
ClientMqtt::ClientMqtt(QObject *parent):QMqttClient(parent),m_device(new WebSocketIODevice(this))
{
    QObject::connect(this,&QMqttClient::errorChanged,this,[=](QMqttClient::ClientError error ){
        qDebug()<<"errormqttt:"<<error;
        this->restart();
    });
    connect(this,&ClientMqtt::node_address_changed,this,[=](){
        this->restart();
    });
    connect(m_device, &WebSocketIODevice::socketConnected, this, [this]() {
        this->setTransport(m_device, QMqttClient::IODevice);
        this->connectToHost();
    });

};

ResponseMqtt*  ClientMqtt::get_subscription(const QString& topic)
{
    auto var=subscribe(QMqttTopicFilter(topic));
    return new ResponseMqtt(var);
}

ResponseMqtt* ClientMqtt::get_outputs_unlock_condition_address(const QString& condition_address)
{
    return get_subscription("outputs/unlock/"+condition_address);
}
ResponseMqtt* ClientMqtt::get_outputs_outputId(const QString& id)
{
    return get_subscription("outputs/"+id);
}
ResponseMqtt* ClientMqtt::get_blocks(void)
{
    return get_subscription("blocks");
}
}
