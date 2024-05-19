#ifndef CLIENT_H
#define CLIENT_H

#include <QMap>
#include <QTcpSocket>
#include "field.hpp"

class Client
{
public:
    Client();
    ~Client();

    typedef QMap<int, Client>::iterator ClientIterator;

    enum ClientStatus
    {
        ST_DISCONNECTED = 0,
        ST_CONNECTED       ,
        ST_AUTHORIZED      ,
        // more
    };

    enum Readiness
    {
        ST_NREADY       = 0,
        ST_READY           ,
        ST_PLAYING         ,
    };

    void setLogin(const QString& login);
    QString getLogin();
    bool isAuthorized() const;
    void updateState(ClientStatus state);

    void initField();
    void initField(QString field);
    QString getFieldStr();
    bool isCellEmpty(int x, int y);

public:
    QTcpSocket*  socket_;
    ClientStatus status_;
    Readiness readiness_;
    ClientIterator enemy_;
    QString login_;

private:
    Field* field_;
};

typedef QMap<int, Client> Clients;
typedef Clients::iterator ClientsIterator;

#endif // CLIENT_H
