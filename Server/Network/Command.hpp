#pragma once

#include "../config.hpp"

namespace pms
{
    enum ClientCommandType
    {
        CmdSetUserID,
        CmdRequestPomemeons,
        CmdPick,
        CmdPlace,
        CmdSetPMData,
        CmdStopServer,
        CmdUpdatePos,
        CmdRequestUserData
    };

    enum ServerCommandType
    {
        SCmdUserID,
        SCmdDisconnect,
        SCmdCashStat,
        SCmdRequestPMData,
        SCmdUserData,
        SCmdPomemeon,
        SCmdErr
    };

    class Command
    {
    public:
        static char terminating;

        string command;
        vector<string> args;

        Command(string commandString);
        Command(ServerCommandType cmdType, initializer_list<string> arglist);
        Command();

        void addArgument(string arg);
        string getArgument(int argId);
        void removeArgument(int argId);
        void fromString(string commandString);
        string toString();

        bool isEqual(string cmd, int argc);
    };
}
