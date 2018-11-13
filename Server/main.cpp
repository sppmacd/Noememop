#include "config.hpp"
#include "PMSServer.hpp"

#include <windows.h>

namespace pms
{
    void sendStats(string statString)
    {
        ofstream stream;
        stream.open("stats.txt", std::ios::trunc);
        stream << statString;
        stream.close();
    }
}

int main()
{
    try
    {
        CreateProcess("PomemeonStatManager.exe", NULL, NULL, NULL, FALSE, 0, NULL, NULL, NULL, NULL);
        pms::PMSServer::getInstance()->start();
    }
    catch(exception exc)
    {
        pms::log(pms::Fatal, string("Exception caught: ") + exc.what());
        return 1;
    }
    return 0;
}
