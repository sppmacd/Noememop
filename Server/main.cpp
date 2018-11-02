#include "config.hpp"
#include "PMSServer.hpp"

int main()
{
    pms::PMSServer::getInstance()->start();
    return 0;
}
