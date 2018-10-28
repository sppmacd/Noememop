#include "config.hpp"
#include <windows.h>

namespace pms
{
    void displayColored(int colorCode, string str)
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorCode);
        cout << str;
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
    }

    void log(LogLevel level, string str)
    {
        time_t t = time(NULL);
        string ct = ctime(&t);

        switch(level)
        {
        case Info:
            displayColored(0x0F, "INFO: [" + ct.substr(0, ct.size() - 2) + "] ");
            displayColored(0x07, str);
            break;
        case Warning:
            displayColored(0x0E, "WARN: [" + ct.substr(0, ct.size() - 2) + "] ");
            displayColored(0x06, str);
            break;
        case Error:
            displayColored(0x0C, "ERROR: [" + ct.substr(0, ct.size() - 2) + "] ");
            displayColored(0x04, str);
            break;
        case Debug:
            displayColored(0x08, "DEBUG: [" + ct.substr(0, ct.size() - 2) + "] ");
            displayColored(0x08, str);
            break;
        }
        cout << endl;
    }
}
