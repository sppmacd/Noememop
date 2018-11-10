#include "config.hpp"
#include "PMSServer.hpp"

#include <windows.h>

namespace pms
{
    HANDLE statPipeIn_W, statPipeIn_R;
    HANDLE statPipeOut_W, statPipeOut_R;
    SECURITY_ATTRIBUTES sats = {0};
    PROCESS_INFORMATION info = {0};
    STARTUPINFO sti = {0};

    /// thanks to FamTrinli
    bool startProcess(string name, string args)
    {
        statPipeIn_W = statPipeIn_R = statPipeOut_W = statPipeOut_R = NULL;
        sats.bInheritHandle = TRUE;
        sats.lpSecurityDescriptor = NULL;
        sats.nLength = sizeof(sats);

        CreatePipe(&statPipeIn_R, &statPipeIn_W, &sats, 0);
        CreatePipe(&statPipeOut_R, &statPipeOut_W, &sats, 0);

        sti.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
        sti.wShowWindow = SW_SHOW;
        sti.hStdInput = statPipeIn_R;
        sti.hStdOutput = statPipeOut_W;
        sti.hStdError = statPipeOut_W;

        return CreateProcessA(name.c_str(), LPSTR(string(name + " " + args).c_str()), NULL, NULL, TRUE, 0, NULL, NULL, &sti, &info);
    }

    BYTE buffer[2048];
    DWORD writ, excode, read, available;

    /// thanks to FamTrinli
    void sendStats(string statString)
    {
        std::string str;
		std::string errStr;

        WriteFile(statPipeIn_W, statString.c_str(), statString.size(), &writ, NULL);
        Sleep(100);

        PeekNamedPipe(statPipeOut_R, buffer, sizeof(buffer), &read, &available, NULL);
        do
        {
            ZeroMemory(buffer, sizeof(buffer));
            if(!ReadFile(statPipeOut_R, buffer, sizeof(buffer), &read, NULL) || !read)
                break;
            buffer[read] = 0;
            str+=(char*)buffer;
        }
        while(read >= sizeof(buffer));

		if(!str.empty()) log(Verbose, "[Stat Saver] " + str);
    }
}

int main()
{
    try
    {
        if(!pms::startProcess("PomemeonStatManager.exe", to_string(time(NULL))+"\0"))
        {
            pms::log(pms::Fatal, "Couldn't start stat manager: " + to_string(GetLastError()));
        }
        pms::PMSServer::getInstance()->start();
    }
    catch(exception exc)
    {
        pms::log(pms::Fatal, string("Exception caught: ") + exc.what());
    }
    return 0;
}
