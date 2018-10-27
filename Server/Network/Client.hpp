#pragma once

namespace pms
{
    struct Client
    {
        int userID;
        TcpSocket* socket;

        Client(int id, TcpSocket* sck) : userID(id), socket(sck) {}
    };
}
