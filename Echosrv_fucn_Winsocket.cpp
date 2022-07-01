//#include "pch.h"
//#include "stdafx.h"

#pragma comment(lib,"ws2_32.lib")

#include <winsock2.h>
#include <iostream>
#include <atomic>
//#include <thread>

#pragma warning(disable: 4996)

int MaxClients = 3;
std :: atomic<int> Counter = 0;

void ClientHandler(SOCKET Connection);
void Connect(char* ipaddrhost, int porthost);
static void SetLib();

int main(int argc, char* argv[]) {
    SetLib();

    char ipaddrhost[15]; // 127.0.0.1 :localhost
    std :: cout << "Enter ip adress for host your server: ";
    std :: cin >> ipaddrhost;

    unsigned int porthost; //telnet
    std :: cout << "Enter port host: ";
    std :: cin >> porthost;

    if (!porthost)
        porthost = 23;

    Connect(ipaddrhost, porthost);

    system("pause");
    return 0;
}


void Connect(char* ipaddrhost, int porthost) {
    SOCKADDR_IN addr;
    int sizeofaddr = sizeof(addr);
    addr.sin_addr.s_addr = inet_addr(ipaddrhost);
    addr.sin_port = htons(porthost);
    addr.sin_family = AF_INET;

    SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);
    if(:: bind(sListen, (SOCKADDR*)&addr, sizeof(addr)) < 0){
        std :: cout << "Error bind\n";
        exit(2);
    }
    if(:: listen(sListen, SOMAXCONN) < 0){
        std :: cout << "Error listen\n";
        exit(3);
    }

    //std :: thread StreamClientHandler;

    SOCKET newConnection;
    while (true) {
        newConnection = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr);
        if (newConnection == 0) {
           std :: cout << "Disconnet" << std :: endl;
           closesocket(newConnection);
           continue;
        }
        if (Counter < MaxClients) {
                    const char * message = "Welcome to Echo!\n";
                    std :: cout << "Connect " << Counter << std :: endl;
                    send(newConnection, message, strlen(message), NULL);
                    Counter++;
                    CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler,(LPVOID) newConnection, NULL, NULL);
                   /* StreamClientHandler = std :: thread(&ServerEcho :: ClientHandler, this, newConnection);
                    StreamClientHandler.detach();*/
        }
        else {
          std :: cout << "Error connection " << std :: endl;
            const char * message = "Server is full. Try later. Echo. \n \n";
            send(newConnection, message, strlen(message), NULL);
            closesocket(newConnection);
            newConnection = NULL;
        }
    }
}

void ClientHandler(SOCKET Connection) {
    while (true) {
        char message[1];
        if (recv(Connection, message, 1, NULL) == 0)
            break;
        if(send(Connection, message, 1, NULL) == 0)
            break;
    }
    Counter--;
    std :: cout << "Disconect client " << Counter << std :: endl;
    closesocket(Connection);
    Connection = NULL;
}

static void SetLib(){
    WSAData wsadata;
    WORD DLLVersion = MAKEWORD(2, 1);
    if (WSAStartup(DLLVersion, &wsadata) != 0)
    {
       std :: cout << "Error lib" << std :: endl;
        exit(1);
    }
}

