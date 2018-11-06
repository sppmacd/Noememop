#ifndef CONFIG_HPP_INCL
#define CONFIG_HPP_INCL

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <string>
#include <vector>
#include <fstream>
using namespace std;

#include <SFML/Network.hpp>
using namespace sf;

namespace pms
{
    enum LogLevel
    {
        Debug,
        Info,
        Warning,
        Error
    };

    void displayColored(int colorCode, string str);
    void log(LogLevel level, string str);
}

#endif // CONFIG_HPP_INCL

