#include"client/qclient.hpp"
#include"client/qclientMQTT.hpp"
#include"pow/qpow.hpp"
#include"block/qblock.hpp"
#include"encoding/qbech32.hpp"
#include"crypto/qed25519.hpp"
#include"crypto/qslip10.hpp"
#include<iostream>
#include <QCoreApplication>
#include<QJsonDocument>
#undef NDEBUG
#include <assert.h>


using namespace qiota::qblocks;
using namespace qiota::qpow;
using namespace qiota;
using namespace qcrypto;
using namespace qencoding::qbech32;

int main(int argc, char** argv)
{

    QCoreApplication a(argc, argv);
    //auto iota=new Client(QUrl("https://api.testnet.shimmer.network"));
    auto iotaMqtt=new ClientMqtt(QUrl("wss://api.testnet.shimmer.network:443/api/mqtt/v1"));


    QObject::connect(iotaMqtt,&QMqttClient::stateChanged,iotaMqtt,[=](QMqttClient::ClientState state ){
        if(iotaMqtt->state()==QMqttClient::Connected)
        {
            auto resp=iotaMqtt->get_outputs_unlock_condition_address("address/rms1qpn3fx68e5m7ade2dfnm7e2szcpg9wsxmx2jjcurgu6e50hwkaa054gcnwt");

            QObject::connect(resp,&ResponseMqtt::returned,iotaMqtt,[=]( QJsonValue data ){
                qDebug()<<"new output:"<<data;
            });
        }
    });

        return a.exec();
    }
