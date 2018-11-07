#include "DataFile.hpp"
#include "../Network/Command.hpp"

namespace pms
{
    bool DataFile::openFile(string fileName, bool save)
    {
        if(save) fileHandlerOut.open(fileName, ios::trunc);
        else fileHandlerIn.open(fileName, ios::trunc);
        
        if(save) return fileHandlerOut.good();
        else return fileHandlerIn.good();
    }

    DataFile::DataFile(DataType type)
    {
        dataType = type;
    }

    int DataFile::getSize()
    {
        if(openFile("Data/" + to_string(dataType) + "_size.data", false))
        {
        string str = fileHandlerIn.getline();

        return stoi(str);
        }
        else
            return 0;
    }

    void DataFile::saveSize(int newSize)
    {
        
        if(openFile("Data/" + to_string(dataType) + "_size.data", true))
        {
        fileHandlerOut << newSize;
        }

    }

    DataNode DataFile::getNode(int id)
    {
        int cnt = getSize();
        if(id < cnt)
        {
            if(!openFile("Data/" + to_string(dataType) + "_" + to_string(id) + ".data", false)) return DataNode{};

            string str = fileHandlerIn.getline();

            DataNode node;

            // parse data
            int lastp = 0;

            for(unsigned int i = 0; i < str.size()+1; i++)
            {
                if(str[i] == Command::terminating || str[i] == '\0' || i == str.size())
                {
                    node.args.push_back(str.substr(lastp, i-lastp));

                    lastp = i+1;
                }
            }

            fileHandlerIn.close();

            return node;
        }
        return DataNode{};
    }

    void DataFile::setNode(DataNode node, int id)
    {
        int cnt = getSize();

        if(id < cnt)
        {
            if(!openFile("Data/" + to_string(dataType) + "_" + to_string(id) + ".data", true)) return;
        for(string& arg: node.args)
        {
            if(cnt != 0)
                fileHandlerOut << Command::terminating+arg;
            else
                fileHandlerOut << arg;
            cnt++;
        }

        fileHandlerOut.close();
        }
    }
}
