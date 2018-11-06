#pragma once

#include "config.hpp"

namespace pms
{
    class Command
    {
        string command;
        vector<string> args;

    public:
        Command(string commandString);
        Command();

        void addArgument(string arg);
        string getArgument(int argId);
        void removeArgument(int argId);
        void fromString(string commandString);
        string toString();
    };
}
