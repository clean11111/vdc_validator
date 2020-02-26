/* Version History
 * 1.0 : Init
*/

#include <QtWidgets>
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    srand((unsigned int)time(NULL));
    QLabel *serverStateLabel = new QLabel(tr("Server State :"));
    serverState = new QLineEdit;
    serverState->setPlaceholderText(tr("Not Initialized"));
    serverState->setReadOnly(true);

    QLabel *clientStateLabel = new QLabel(tr("Client State :"));
    clientState = new QLineEdit;
    clientState->setPlaceholderText(tr("Not Initialized"));
    clientState->setReadOnly(true);

    QLabel *fileloadStateLabel = new QLabel(tr("FileLoad State :"));
    fileloadState = new QLineEdit;
    fileloadState->setPlaceholderText(tr("Not Initialized"));
    fileloadState->setReadOnly(true);

    QLabel *validateStateLabel = new QLabel(tr("Validation State :"));
    validateState = new QLineEdit;
    validateState->setPlaceholderText(tr("Not Started"));
    validateState->setReadOnly(true);

    QVBoxLayout *StartbuttonLayout = new QVBoxLayout;
    startButton = new QPushButton(tr("&Start Server"));
    StartbuttonLayout->addWidget(startButton);

    QVBoxLayout *LoadbuttonLayout = new QVBoxLayout;
    loadButton = new QPushButton(tr("&Load Json"));
    LoadbuttonLayout->addWidget(loadButton);

    QVBoxLayout *ClearbuttonLayout = new QVBoxLayout;
    clearButton = new QPushButton(tr("&Clear"));
    ClearbuttonLayout->addWidget(clearButton);

    progress = new QProgressBar();
    progress->setMaximum(100);
    progress->setValue(50);
    progress->setFixedHeight(100);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    QGridLayout *infoLayout = new QGridLayout;
    mainLayout->setAlignment(Qt::AlignVCenter);
    infoLayout->addWidget(serverStateLabel, 0, 0);
    infoLayout->addWidget(serverState, 0, 1);
    infoLayout->addLayout(StartbuttonLayout, 0, 2);
    infoLayout->addWidget(clientStateLabel, 1, 0);
    infoLayout->addWidget(clientState, 1, 1);
    infoLayout->addLayout(LoadbuttonLayout, 1, 2);
    infoLayout->addWidget(fileloadStateLabel, 2, 0);
    infoLayout->addWidget(fileloadState, 2, 1);
    infoLayout->addLayout(ClearbuttonLayout, 2, 2);
    infoLayout->addWidget(validateStateLabel, 3, 0);
    infoLayout->addWidget(validateState, 3, 1);
    mainLayout->addLayout(infoLayout);
    mainLayout->addWidget(progress);
    setLayout(mainLayout);
    setWindowTitle(tr("Delta Validator Tool"));

    connect(startButton, SIGNAL(clicked()), this, SLOT(StartServer()));
    connect(clearButton, SIGNAL(clicked()), this, SLOT(Clear()));
    connect(loadButton, SIGNAL(clicked()), this, SLOT(loadJSON()));
}

MainWindow::~MainWindow()
{
    qDeleteAll(m_clients.begin(), m_clients.end());
}

//! [onNewConnection]
void MainWindow::onNewConnection()
{
    pSocket = m_pWebSocketServer->nextPendingConnection();
    qDebug() << "Client connected:";
    clientState->setText(tr("Connected"));
    connect(pSocket, &QWebSocket::textMessageReceived, this, &MainWindow::processTextMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &MainWindow::socketDisconnected);
    m_clients << pSocket;
}
//! [onNewConnection]

//! [processTextMessage]
void MainWindow::processTextMessage(QString message)
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (pClient)
    {
        bool parse_result;
        Json::Reader reader;
        Json::Value root;
        parse_result = reader.parse(message.toStdString(), root);
        if(parse_result == true) {
            ParseEvent(root);
        }
    }
}
//! [processTextMessage]

//! [socketDisconnected]
void MainWindow::socketDisconnected()
{
    qDebug() << "Client disconnected";
    clientState->setText(tr("Disconnected"));
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (pClient)
    {
        m_clients.removeAll(pClient);
        pClient->deleteLater();
    }
}
//! [socketDisconnected]

void MainWindow::loadJSON()
{
    QString filename =  QFileDialog::getOpenFileName(
          this,
          "Open Json File",
          QDir::currentPath(),
          tr("Json(*.json)"));

    if(!filename.isNull())
    {
        qDebug() << "selected file path : " << filename.toUtf8();
        bool parsingSuccessful;
        Json::Reader reader;
        Json::Value root;
        std::string data;
        std::string buffer;
        std::ifstream openFile(filename.toStdString());
        parsingSuccessful = reader.parse(openFile, root);
        openFile.close();
        if (parsingSuccessful == false) {
            fileloadState->setText(tr("Invalid Json File"));
        } else {
            fileloadState->setText(tr("Json File Open Success"));
            hmi_info = root;
        }
    }
}

void MainWindow::ParseEvent(const Json::Value &request)
{
    std::string key = request.getMemberNames().at(0);
    SendWSMessage();
}

void MainWindow::StartServer()
{
    m_pWebSocketServer = new QWebSocketServer(QStringLiteral("VDC Test Server"),
                                              QWebSocketServer::NonSecureMode,
                                              this);

    quint16 port;
    if(hmi_info["portnumber"].empty() == true) {
        port = 80;
    } else {
        port = hmi_info["portnumber"].asUInt64();
    }

    if (m_pWebSocketServer->listen(QHostAddress::Any, port))
    {        
        qDebug() << "VDC Test Server listening on port :" << m_pWebSocketServer->serverPort();
        QString temp = tr("Listening Port : ") + QString::number(m_pWebSocketServer->serverPort());
        serverState->setText(temp);
        connect(m_pWebSocketServer, &QWebSocketServer::newConnection,
                this, &MainWindow::onNewConnection);
    } else {
        serverState->setText(tr("Listening is failed"));
    }
}

void MainWindow::SendWSMessage()
{
    QString message = QString::fromLocal8Bit(str.c_str());
    qint64 size = pSocket->sendTextMessage(message);
    qDebug() << "size : " << size;
}

void MainWindow::Clear()
{
}

void MainWindow::Test()
{
    Json::Value root = hmi_info["Test"];
    str = styledWriter.write(root);
}
