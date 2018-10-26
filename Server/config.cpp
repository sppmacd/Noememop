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
        time_t t = time(NULL)
        if(level != Debug)
            cout << "[" << ctime(&t) << "] ";
        else
            drawColored(0x08, "[" + ctime(&t) + "] ");

        switch(level)
        {
        case Info:
            cout << "INFO: ";
            displayColored(0x0F, str);
        case Warning:
            cout << "WARN: ";
            displayColored(0x06, str);
        case Error:
            cout << "ERROR: ";
            displayColored(0x0C, str);
        case Debug:
            displayColored(0x08, "DEBUG: " + str);
        }
    }
}
