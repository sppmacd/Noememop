#include "config.hpp"
#include <windows.h>
#include "PMSServer.hpp"

namespace pms
{
    time_t firstLogTime = 0;

    void displayColored(int colorCode, string str)
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorCode);
        cout << str;
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
    }

    void log(LogLevel level, string str)
    {
        time_t t = time(NULL);

        if(firstLogTime == 0)
            firstLogTime = t;

        string ct(ctime(&t));
        string timeString = ct.substr(0, ct.size() - 1);

        switch(level)
        {
        case Info:
            displayColored(0x0A, "INFO: [" + timeString + " (" + to_string(PMSServer::getInstance()->tick) + ")" + "] ");
            displayColored(0x02, str);
            break;
        case Warning:
            displayColored(0x0E, "WARN: [" + timeString + " (" + to_string(PMSServer::getInstance()->tick) + ")" + "] ");
            displayColored(0x06, str);
            break;
        case Error:
            displayColored(0x0C, "ERROR: [" + timeString + " (" + to_string(PMSServer::getInstance()->tick) + ")" + "] ");
            displayColored(0x04, str);
            break;
        case Debug:
            displayColored(0x0F, "DEBUG: [" + timeString + " (" + to_string(PMSServer::getInstance()->tick) + ")" + "] ");
            displayColored(0x07, str);
            break;
        case Fatal:
            displayColored(0x08, "\n-------------------------------------\n");
            displayColored(0x04, "FATAL: [" + timeString + " (" + to_string(PMSServer::getInstance()->tick) + ")" + "] ");
            displayColored(0x04, str);
            displayColored(0x08, "\n-------------------------------------\n");
            break;
        case ImportantInfo:
            displayColored(0x0B, "INFO: [" + timeString + " (" + to_string(PMSServer::getInstance()->tick) + ")" + "] ");
            displayColored(0x03, str);
            break;
        case Verbose:
            displayColored(0x08, "VERBOSE: [" + timeString + " (" + to_string(PMSServer::getInstance()->tick) + ")" + "] ");
            displayColored(0x08, str);
            break;
        }
        cout << endl;
    }
}
