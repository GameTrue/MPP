#pragma once

#include <QObject>
#include <QTcpSocket>
#include <QMutex>
#include <QQueue>
#include <QString>

class NetworkWorker : public QObject
{
    Q_OBJECT
    
public:
    explicit NetworkWorker(QObject* parent = nullptr);
    ~NetworkWorker();
    
    bool isConnected() const;
    
public slots:
    void connectToServer(const QString& address, quint16 port);
    void disconnectFromServer();
    void sendCommand(const QString& command);
    
signals:
    void connected();
    void disconnected();
    void errorOccurred(const QString& errorMessage);
    void responseReceived(const QString& response);
    
private slots:
    void onReadyRead();
    void onSocketError(QAbstractSocket::SocketError socketError);
    
private:
    QTcpSocket* socket_;
    QMutex mutex_;
};
