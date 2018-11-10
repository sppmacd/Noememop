#include "DataFile.hpp"
#include "../Network/Command.hpp"

namespace pms
{
    string getline(ifstream& str)
    {
        string gl;
        std::getline(str, gl);
        return gl;
    }

    bool DataFile::openFile(string fileName, bool save)
    {
        //log(Info, "Opening file: '" + fileName + "'");
        if(save)
        {
            fileHandlerOut.open(fileName, ios::trunc);
            bool x = fileHandlerOut.good();
            return x;
        }
        else
        {
            fileHandlerIn.open(fileName);
            bool x = fileHandlerIn.good();
            return x;
        }
    }

    DataFile::DataFile(DataType type)
    {
        dataType = type;
    }

    int DataFile::getSize()
    {
        if(openFile("Data/" + to_string(dataType) + "_size.data", false))
        {
            string str = getline(fileHandlerIn);

            fileHandlerIn.close();

            return stoi(str);
        }
        else
            return 0;
    }

    void DataFile::saveSize(int newSize)
    {
        if(openFile("Data/" + to_string(dataType) + "_size.data", true))
        {
            fileHandlerOut << newSize << endl;
        }
    }

    DataNode DataFile::getNode(int id)
    {
        if(!openFile("Data/" + to_string(dataType) + "_" + to_string(id) + ".data", false))
            return DataNode{};

        string str = getline(fileHandlerIn);

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
            //log(Error, node.args[i]);
        }

        fileHandlerIn.close();

        return node;
    }

    void DataFile::setNode(DataNode node, int id)
    {
        if(!openFile("Data/" + to_string(dataType) + "_" + to_string(id) + ".data", true))
            return;

        int cnt2 = 0;

        for(string& arg: node.args)
        {
            if(cnt2 != 0)
                fileHandlerOut << Command::terminating+arg;
            else
                fileHandlerOut << arg;
            cnt2++;
        }
        fileHandlerOut << endl;
        fileHandlerOut.close();
    }
}
