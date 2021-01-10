#include "server.h"

int main()
{
    Server myServer(8888);
    myServer.start();
    return 0;
}