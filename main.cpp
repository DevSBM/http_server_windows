#include "./server/server.h"
#include "./client/client.h"


void runServer() {
    Server s;
}

void runClient() {
    Client c;
}


int main(int argc, char* argv[])
{

    if ((argc > 1)) {
        if (std::string(argv[1]) == "server") {
            runServer();
        }
        else {
            runClient();
        }
    }
}