#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QPoint>
#include <QTcpSocket>
#include <QRegularExpression>
#include <QMessageBox>
#include <QFile>
#include "config.h"
#include "constants.h"
#include "model.h"

enum GameResult
{
    GR_NONE = 0 ,
    GR_WON  = 1 ,
    GR_LOST = -1
};

enum GameErrorMessage
{
    GEM_UNKNOWN                 = -1,
//    GEM_NONE                    = 0 ,
//    GEM_ALREADY_CONNECTED           ,
//    GEM_WRONG_FIELD                 ,
//    GEM_WRONG_USER                  ,
//    GEM_SERVER_ERROR                ,
//    GEM_SERVER_CONNECTION_REFUSED   ,
//    GEM_SERVER_CONNECTION_TIMEOUT   ,
//    GEM_SERVER_UNAVAILABLE          ,
};

class Controller: public QWidget
{
    Q_OBJECT
public:
    Controller(Model* model);
    ~Controller();
    void onMousePressed(const QPoint& position, QMouseEvent* event);

private:
    QTcpSocket* client_;
    Model* model_;
//    QHostAddress serverAddress;
//    quint16 serverPort;
//    bool connectionError;
};

#endif // CONTROLLER_H