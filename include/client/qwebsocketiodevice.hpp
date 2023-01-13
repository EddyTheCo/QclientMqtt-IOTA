#ifndef WEBSOCKETIODEVICE_H
#define WEBSOCKETIODEVICE_H

#include <QtCore/QIODevice>
#include <QtWebSockets/QWebSocket>
#include<QByteArray>

class WebSocketIODevice : public QIODevice
{
    Q_OBJECT
public:
    WebSocketIODevice(QObject *parent = nullptr);

    bool open(OpenMode mode) override;
    void close() override;

    qint64 readData(char *data, qint64 maxlen) override;
    qint64 writeData(const char *data, qint64 len) override;

    void setUrl(const QUrl &url);
    void setProtocol(const QByteArray &data);
Q_SIGNALS:
    void socketConnected();

public slots:
    void handleBinaryMessage(const QByteArray &msg);
    void onSocketConnected();

private:
    QByteArray m_protocol;
    QByteArray m_buffer;
    QWebSocket m_socket;
    QUrl m_url;
};

#endif // WEBSOCKETIODEVICE_H
