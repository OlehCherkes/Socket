#ifndef MYSERVER_H
#define MYSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QFile>

//собственный об'экт класа QTcpServer
class myserver: public QTcpServer
{
    Q_OBJECT
public:
    myserver();
    ~myserver();

    QTcpSocket* socket;
    QByteArray Data;

    QJsonDocument doc;
    QJsonParseError docError;

//проотипы функций
public slots:
    void startServer(); //метод запуска сервера (привем/передача данны)
    void incomingConnection(int socketDescriptor); //описание действий клиента
    void sockReady(); //готовность сокета считать полученные данные
    void sockDisc(); //метод овечат за действи€ при отключении клиента
};

#endif // MYSERVER_H
