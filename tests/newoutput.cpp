
#include"client/qclientMQTT.hpp"

#include <QCoreApplication>
#include<QJsonDocument>
#include<QJsonObject>
#undef NDEBUG
#include <assert.h>


using namespace qiota;

int main(int argc, char** argv)
{
    if(argc==3)
    {
        QCoreApplication a(argc, argv);
        auto iotaMqtt=new ClientMqtt(QUrl(QString(argv[1])));    //"wss://api.testnet.shimmer.network:443/api/mqtt/v1";


        QObject::connect(iotaMqtt,&QMqttClient::stateChanged,iotaMqtt,[=](QMqttClient::ClientState state ){
            if(iotaMqtt->state()==QMqttClient::Connected)
            {
                auto resp=iotaMqtt->get_outputs_unlock_condition_address(
                            "address/" + QString(argv[2]));

                QObject::connect(resp,&ResponseMqtt::returned,iotaMqtt,[=]( QJsonValue data ){
                    qDebug().noquote()<<"new output:\n"<<QString(QJsonDocument(data.toObject()).toJson(QJsonDocument::Indented));

                });
            }
        });

        return a.exec();
    }
}
