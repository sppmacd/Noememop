#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        cerr << "Usage: 'PomemeonStatManager <logFile>'" << endl;
        return 1;
    }
    else
    {
        string fileName(argv[1]);

        bool running = true;
        while(running)
        {
            string str;
            cin >> str;

            ofstream file;
            file.open(fileName);

            if(file.good())
            {
                file << str << endl;
                file.close();
                cout << "Successfully saved stats. ";
            }
            else
            {
                cerr << "Cannot save stats: " << strerror(errno);
            }
        }
    }
    return 0;
}
