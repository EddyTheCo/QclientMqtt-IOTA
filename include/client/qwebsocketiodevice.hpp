#ifndef WEBSOCKETIODEVICE_H
#define WEBSOCKETIODEVICE_H

#include <QtCore/QIODevice>
#include <QtWebSockets/QWebSocket>
#include<QByteArray>
#include <QtCore/QtGlobal>

#if defined(WINDOWS_QCMQTT)
# define QCMQTT_EXPORT Q_DECL_EXPORT
#else
#define QCMQTT_EXPORT Q_DECL_IMPORT
#endif

class QCMQTT_EXPORT WebSocketIODevice : public QIODevice
{
	Q_OBJECT
	public:
		WebSocketIODevice(QObject *parent = nullptr);

		bool open(OpenMode mode=QIODevice::ReadWrite) override;
		void close() override;

		qint64 readData(char *data, qint64 maxlen) override;
		qint64 writeData(const char *data, qint64 len) override;

		void setUrl(const QUrl &url);
		void setProtocol(const QByteArray &data);
		signals:
		void socketConnected();

		void handleBinaryMessage(const QByteArray &msg);
		void onSocketConnected();

	private:
		QByteArray m_protocol;
		QByteArray m_buffer;
		QWebSocket m_socket;
		QUrl m_url;
};

#endif // WEBSOCKETIODEVICE_H
