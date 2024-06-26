#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QMessageBox>
#include <QDebug>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QString>
#include <QImage>
#include <QStringList>
#include <QListWidget>
#include <QTextBrowser>
#include <QStackedWidget>
#include "./config.h"
#include "field.h"
#include "model.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

enum ClientState  // states of client
{
    ST_DISCONNECTED = 0,
    ST_CONNECTED       ,
    ST_AUTHORIZED      ,
    ST_READY           ,
    // more
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QString ip, quint16 port, QWidget *parent = nullptr);
    ~MainWindow();
    void screenUpdate();
    void stateUpdate(ClientState new_state);
    void updateAll();
    void exitFromServer();

private:
    ClientState state_;
    QString ip_;
    int port_;
    QTcpSocket* socket_; // socket for interacting with server
    QByteArray data_;    // data from socket and for socket
    QString login_;
    Model* model_;
    int timerId_;

protected:
    void timerEvent(QTimerEvent *event) override;
    void paintEvent(QPaintEvent* event) override;
    void closeEvent(QCloseEvent* event) override;

public:
    QStringList userLogins_;
    void connectUser();
    void authenticateUser();
    void handleData();
    void usersListUpdate();
    void sendMessage();
    void handleMessageRequest();
    void handleUsersRequest();
    void handlePingRequest();
    void handleExitRequest();
    void updateChats();
    void stopClient(QString msg);

    QImage getFieldImage(const Field& field) const;

public slots:
    void on_sockConnect();
    void on_receiveData();
    void on_sockDisconnect();
    void on_sockError(QAbstractSocket::SocketError error);

private slots:
    void on_connectToServerButton_clicked();
    void on_loginLabel_cursorPositionChanged(int arg1, int arg2);        
    void on_updateButton_clicked();
    void on_sendMessageButton_clicked();
    void on_messageRecieversOptionList_itemSelectionChanged();
    void createTextBrowser(QListWidgetItem* user);

private:
    QListWidget* receiversListWidget;    // user from the list who we want to send the message
    QStackedWidget* receiverBrowserStackedWidget;  // browser (chat) with this user
    QMap<QListWidgetItem*, QTextBrowser*> browserMap;   // map of the user logins and chats with them

private:
    Ui::MainWindow* ui;
};

#endif // MAINWINDOW_H
