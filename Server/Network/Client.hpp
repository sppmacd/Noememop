#pragma once

namespace pms
{
    struct Client
    {
        TcpSocket* socket;
        int userID;

        Client(int id, TcpSocket* sck) : userID(id), socket(sck) {}
    };
}
