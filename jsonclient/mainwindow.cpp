#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    socket = new QTcpSocket(this); //создание нового сокита
        connect(socket,SIGNAL(readyRead()),this,SLOT(sockReady())); //связь по сокиту
        connect(socket,SIGNAL(disconnected()),this,SLOT(sockDisc()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    socket->connectToHost("127.0.0.1",5555);
}

//удаление сокита
void MainWindow::sockDisc()
{
    socket->deleteLater();
}

//готовность сокита на чтение
void MainWindow::sockReady()
{
    //проверка на устойчивость соединения
    if (socket->waitForConnected(500))
    {
        socket->waitForReadyRead(500);
        Data = socket->readAll();
        doc = QJsonDocument::fromJson(Data, &docError);
        //проверка соединения
         if (docError.errorString().toInt() == QJsonParseError::NoError)
        {
            if ((doc.object().value("type").toString() == "connect") && (doc.object().value("status").toString() == "yes"))
            {
                QMessageBox::information(this,  "Information", "Connected");
            }
            else if (doc.object().value("type").toString() == "resultSelect")
            {
                QStandardItemModel* model = new QStandardItemModel(nullptr);
                model->setHorizontalHeaderLabels(QStringList()<<"name");

                QJsonArray docAr = doc.object().value("result").toArray();
                for (int i = 0; i < docAr.count(); i++)
                {
                    QStandardItem* col = new QStandardItem(docAr[i].toObject().value("name").toString());
                    model->appendRow(col);
                }
                ui->tableView->setModel(model);

            }
            else
            {
                QMessageBox::information(this,  "Information", "No connected");
            }
        }
        else
        {
            QMessageBox::information(this,  "Information", "Error" + docError.errorString());
        }
    }
}

//отправка команды на получение всего списка машин
void MainWindow::on_pushButton_2_clicked()
{
    if (socket->isOpen())
    {
        socket->write("{\"type\":\"select\",\"params\":\"cars\"}");
        socket->waitForBytesWritten(500);
    }
    else
    {
        QMessageBox::information(this, "information", "No connection");
    }
}



