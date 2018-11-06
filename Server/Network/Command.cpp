#include "Command.hpp"

namespace pms
{
    char Command::terminating = '|';

    Command::Command(string commandString)
    {
        this->fromString(commandString);
    }

    void Command::addArgument(string arg)
    {
        this->args.push_back(arg);
    }

    string Command::getArgument(int argId)
    {
        if(argId < this->args.size())
            return this->args[argId];
        return "";
    }

    void Command::removeArgument(int argId)
    {
        if(argId < this->args.size())
            this->args.erase(this->args.begin() + argId);
    }

    void Command::fromString(string commandString)
    {
        int lastp = 0;

        for(unsigned int i = 0; i < commandString.size()+1; i++)
        {
            if(commandString[i] == terminating || commandString[i] == '\0' || i == commandString.size())
            {
                if(lastp == 0)
                    command = commandString.substr(lastp, i-lastp);
                else
                    this->args.push_back(commandString.substr(lastp, i-lastp));

                lastp = i+1;
            }
        }
    }

    bool Command::isEqual(string cmd, int argc)
    {
        return cmd == command && this->args.size() == argc;
    }

    Command::Command(ServerCommandType cmdType, std::initializer_list<string> arglist)
    {
        switch(cmdType)
        {
        case SCmdCashStat:
            command = "pms:cashstat"; break;
        case SCmdDisconnect:
            command = "pms:disconnect"; break;
        case SCmdErr:
            command = "pms:err"; break;
        case SCmdPomemeon:
            command = "pms:pomemeon"; break;
        case SCmdRequestPMData:
            command = "pms:requestpmdata"; break;
        case SCmdUserData:
            command = "pms:userdata"; break;
        case SCmdUserID:
            command = "pms:userid"; break;
        }

        this->args = arglist;
    }

    string Command::toString()
    {
        string str = command;

        for(string& arg: args)
            command += terminating+arg;

        return str + "\n\0";
    }
}
