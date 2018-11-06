#include "Command.hpp"

namespace pms
{
    char terminating = '|';

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

    bool isEqual(string cmd, int argc)
    {
        return cmd = command && this->args.size() == args;
    }

    Command(ServerCommandType cmdType, initializer_list<string> args)
    {
        switch(cmdType)
        {
            case Cmd
        }
    }

    string Command::toString()
    {
        string str = command;

        for(string& arg: args)
            command += terminating+arg;

        return str + "\n\0";
    }
}
