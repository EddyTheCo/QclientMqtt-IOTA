#include"client/qclientMQTT.hpp"
#include<QJsonDocument>
#include<QJsonObject>
#include<QJsonValue>
#include<iostream>
namespace qiota{

    ResponseMqtt::ResponseMqtt(QMqttSubscription * thesub):sub_(thesub)
	{
        QObject::connect(sub_,&QMqttSubscription::messageReceived ,this, &ResponseMqtt::fill);
	}
    void ResponseMqtt::fill(QMqttMessage msg)
	{
        qDebug()<<"message recieved";
		emit returned((QJsonDocument::fromJson(msg.payload())).object());
	}

    ClientMqtt::ClientMqtt(const QUrl &url):QMqttClient(),node_address_(url)
	{

        QObject::connect(this,&QMqttClient::stateChanged,this,[=](QMqttClient::ClientState state ){
            qDebug()<<"state:"<<state;
        });

        QObject::connect(this,&QMqttClient::errorChanged,this,[=](QMqttClient::ClientError error ){
            qDebug()<<"error:"<<error;
        });


            m_device.setUrl(node_address_);
            if (!m_device.open(QIODevice::ReadWrite))
                    qDebug() << "Could not open socket device";

            connect(&m_device, &WebSocketIODevice::socketConnected, this, [this]() {
                this->setTransport(&m_device, QMqttClient::IODevice);
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
    ResponseMqtt* ClientMqtt::get_blocks(void)
    {
        return get_subscription("blocks");
    }
}
