#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <ctime>

using namespace std;

const int port = 3425;

int main()
{
    int sTimeClient, /* дискриптор клиентского сокета */
        sTimeServer; /* дискриптор серверного сокета */
    struct sockaddr_in addr;
    time_t currentTime;
    ofstream fout;

    sTimeServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(sTimeServer < 0)
    {
        perror("socket");
        return -1;
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(sTimeServer, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        return -2;
    }

    listen(sTimeServer, 1);


    while(1)
    {
            fout.open("test.log", ios_base::app | ios_base::out);
            sTimeClient = accept(sTimeServer, NULL, NULL);
            if(sTimeClient < 0) {
                    perror("accept");
                    return -3;
                }
            currentTime = time(NULL);
            send(sTimeClient, (void *) currentTime, sizeof(currentTime), 0);

            cout << "request at: " << ctime(&currentTime);
            fout << "request at: " << ctime(&currentTime);


            close(sTimeClient);
            fout.close();
        }

    close(sTimeServer);

    return 0;
}
