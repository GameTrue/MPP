#include "NetworkWorker.h"

NetworkWorker::NetworkWorker(QObject* parent)
    : QObject(parent)
    , socket_(new QTcpSocket(this))
{
    connect(socket_, &QTcpSocket::connected, this, &NetworkWorker::connected);
    connect(socket_, &QTcpSocket::disconnected, this, &NetworkWorker::disconnected);
    connect(socket_, &QTcpSocket::readyRead, this, &NetworkWorker::onReadyRead);
    connect(socket_, &QAbstractSocket::errorOccurred, this, &NetworkWorker::onSocketError);
}

NetworkWorker::~NetworkWorker()
{
    if (socket_->state() == QAbstractSocket::ConnectedState) {
        socket_->disconnectFromHost();
    }
}

bool NetworkWorker::isConnected() const
{
    return socket_->state() == QAbstractSocket::ConnectedState;
}

void NetworkWorker::connectToServer(const QString& address, quint16 port)
{
    QMutexLocker locker(&mutex_);
    if (socket_->state() == QAbstractSocket::ConnectedState) {
        return;
    }
    
    socket_->connectToHost(address, port);
}

void NetworkWorker::disconnectFromServer()
{
    QMutexLocker locker(&mutex_);
    if (socket_->state() == QAbstractSocket::ConnectedState) {
        socket_->disconnectFromHost();
    }
}

void NetworkWorker::sendCommand(const QString& command)
{
    QMutexLocker locker(&mutex_);
    if (socket_->state() != QAbstractSocket::ConnectedState) {
        emit errorOccurred("Not connected to server");
        return;
    }
    
    QByteArray data = command.toUtf8() + "\n";
    socket_->write(data);
}

void NetworkWorker::onReadyRead()
{
    QMutexLocker locker(&mutex_);
    
    QByteArray data = socket_->readAll();
    QString response = QString::fromUtf8(data);
    
    emit responseReceived(response.trimmed());
}

void NetworkWorker::onSocketError(QAbstractSocket::SocketError socketError)
{
    emit errorOccurred(socket_->errorString());
}
