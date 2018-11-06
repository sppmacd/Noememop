#pragma once

#include "../config.hpp"

namespace pms
{
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
        ifstream fileHandlerIn;
        ofstream fileHandlerOut;
    public:
        DataFile(DataType type);
        DataNode getNode(int id);
        void setNode(DataNode node, int id);
        int getSize();
        void saveSize(int newSize);
    };
}
