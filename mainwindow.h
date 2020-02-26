#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QByteArray>
#include <QDebug>
#include <QFileDialog>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkInterface>
#include <QProgressBar>
#include <QWidget>
#include "QtWebSockets/QWebSocket"
#include "QtWebSockets/QWebSocketServer"

#include <unistd.h>
#include <iostream>
#include <string>
#include <fstream>
#include "json.h"

QT_BEGIN_NAMESPACE
class QLabel;
class QLineEdit;
class QTextEdit;
class QProgressBar;
class QPushButton;
QT_END_NAMESPACE

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void ParseEvent(const Json::Value &request);

public slots:
    void StartServer();
    void SendWSMessage();
    void Clear();
    void loadJSON();
    void Test();

private Q_SLOTS:
    void onNewConnection();
    void processTextMessage(QString message);
    void socketDisconnected();

private:
    Json::Value hmi_info;
    Json::StyledWriter styledWriter;
    std::string str;

    QJsonObject jsonObj;
    QLineEdit *serverState;
    QLineEdit *clientState;
    QLineEdit *fileloadState;
    QLineEdit *validateState;
    QProgressBar *progress;
    QPushButton *startButton;
    QPushButton *loadButton;
    QPushButton *sendButton;
    QPushButton *clearButton;
    QPushButton *TestButton;
    QWebSocketServer *m_pWebSocketServer;
    QWebSocket *pSocket;
    QList<QWebSocket *> m_clients;
};

#endif // MAINWINDOW_H
