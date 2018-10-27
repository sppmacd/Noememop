#include "config.hpp"
#include <windows.h>

namespace pms
{
    void displayColored(int colorCode, string str)
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorCode);
        cout << str;
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x00);
    }

    void log(LogLevel level, string str)
    {
        time_t t = time(NULL);
        string ct = ctime(&t);
        if(level != Debug)
            cout << "[" << ct.substr(0, ct.size() - 2) << "] ";
        else
            displayColored(0x08, "[" + ct.substr(0, ct.size() - 2) + "] ");

        switch(level)
        {
        case Info:
            cout << "INFO: ";
            displayColored(0x0F, str);
            break;
        case Warning:
            cout << "WARN: ";
            displayColored(0x06, str);
            break;
        case Error:
            cout << "ERROR: ";
            displayColored(0x0C, str);
            break;
        case Debug:
            displayColored(0x08, "DEBUG: " + str);
            break;
        }
        cout << endl;
    }
}
