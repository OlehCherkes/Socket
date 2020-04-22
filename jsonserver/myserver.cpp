#include "myserver.h"

myserver::myserver(){} //консруктор

myserver::~myserver(){} //диструктор

//страрт сервера
void myserver::startServer()
{
    //проверка запуска сервера
    if (this->listen(QHostAddress::Any,5555)) //порт 5555
    {
        qDebug()<<"Listening";
    }
    else
    {
        qDebug()<<"Not listening";
    }
}

//вxодящее соединение
void myserver::incomingConnection(int socketDescriptor)
{
    socket = new QTcpSocket(this); //новое  соединение
    socket->setSocketDescriptor(socketDescriptor); //уникальный номер (дается автоматически)

        connect(socket,SIGNAL(readyRead()),this,SLOT(sockReady())); //готовность связи
        connect(socket,SIGNAL(disconnected()),this,SLOT(sockDisc())); //розрыв связи

    qDebug()<<socketDescriptor<<" Client connected";

    //ответ клинту статуса соединения
    socket->write("{\"type\":\"connect\",\"status\":\"yes\"}");
    qDebug()<<"Send client connect status - YES";
}

//готовность сервера слушать сеть
void myserver::sockReady()
{
    Data = socket->readAll();
    qDebug() << "Data: " << Data;

    doc = QJsonDocument::fromJson(Data, &docError);

    if (docError.errorString().toInt() == QJsonParseError::NoError)
    {
        if ((doc.object().value("type").toString() == "select") && (doc.object().value("params").toString() == "cars"))
        {
            QFile file;
            file.setFileName("../jsonserver/cars.json");
            if (file.open(QIODevice::ReadOnly|QFile::Text))
            {
                QByteArray fromFile = file.readAll();
                QByteArray itog = "{\"type\":\"resultSelect\",\"result\":" + fromFile + "}";
                socket->write(itog);
                socket->waitForBytesWritten(500);
            }
            file.close();
        }
    }
}


//отключение от сервера
void myserver::sockDisc()
{
    qDebug()<<"Disconnect";
    socket->deleteLater();
}
