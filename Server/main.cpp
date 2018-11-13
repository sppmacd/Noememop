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

    void startProcess(string name, string args)
    {
        STARTUPINFOA sti;
        ZeroMemory(&sti, sizeof(sti));
        PROCESS_INFORMATION pi;
        ZeroMemory(&pi, sizeof(pi));

        char* c = new char[args.size()];
        memcpy(c, args.c_str(), args.size());

        CreateProcessA(name.c_str(), c, NULL, NULL, FALSE, 0, NULL, NULL, &sti, &pi);
    }
}

int main()
{
    try
    {
        pms::startProcess("PomemeonStatManager.exe", "");
        pms::PMSServer::getInstance()->start();
    }
    catch(exception exc)
    {
        pms::log(pms::Fatal, string("Exception caught: ") + exc.what());
        return 1;
    }
    return 0;
}
