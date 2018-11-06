#pragma once

#include "../config.hpp"

namespace pms
{
    struct Sifstream : public ifstream
    {
        string getline()
        {

        }
    };

    struct Sofstream
    {

    };

    enum DataType
    {
        DTPlayer,
        DTPomemeon/*,
        DTHistoryObject*/
    };

    struct DataNode
    {
        vector<string> args;
    };

    class DataFile
    {
        DataType dataType;
        void openFile(string fileName, bool save);
        Sifstream fileHandlerIn;
        Sofstream fileHandlerOut;
    public:
        DataFile(DataType type);
        DataNode getNode(int id);
        void setNode(DataNode node, int id);
        int getSize();
        void saveSize(int newSize);
    };
}
