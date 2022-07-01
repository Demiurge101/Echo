#include "pch.h"
//#include "stdafx.h"

#pragma comment(lib,"ws2_32.lib")

#include <winsock2.h>
#include <iostream>
#include <thread>
#include <atomic>

#pragma warning(disable: 4996)

static void SetLib();


class BaseServer {
    char ipaddrhost[15]; // 127.0.0.1 :localhost
    unsigned short porthost; //telnet
protected:
    int MaxClients = 3;
    std::atomic<int> Counter = 0;
    SOCKADDR_IN addr;
    int sizeofaddr = sizeof(addr);
    SOCKET sListen;
public:
    bool status = true;
    void SetValues();
    void SetMaxClients(int MaxClients) { this->MaxClients = MaxClients; }
    void Start();
    virtual void Close();

    ~BaseServer() {
        if (status) {
            closesocket(sListen);
            WSACleanup();
            Counter = 0;
            std::cout << "Server was stoped. \n";
        }
    }

};

void BaseServer::SetValues() {
    std::cout << "Enter ip adress for host your server: ";
    std::cin >> ipaddrhost;

    std::cout << "Enter port host: ";
    std::cin >> porthost;

    if (!porthost)
        porthost = 23;

}

void BaseServer::Start() {

    addr.sin_addr.s_addr = inet_addr(ipaddrhost);
    addr.sin_port = htons(porthost);
    addr.sin_family = AF_INET;
    sListen = socket(AF_INET, SOCK_STREAM, NULL);
    if (::bind(sListen, (SOCKADDR*)&addr, sizeof(addr)) < 0) {
        std::cout << "Error bind\n";
        exit(2);
    }
    if (::listen(sListen, SOMAXCONN) < 0) {
        std::cout << "Error listen\n";
        exit(3);
    }

    std::cout << "Server is started \n";
}

void BaseServer::Close() {
    if (status) {
        status = false;
        closesocket(sListen);
        WSACleanup();
        Counter = 0;
        std::cout << "Server was stoped. \n";
    }
}

class Echo : public BaseServer {
    SOCKET newConnection;
    void Connect();
    void ClientHandler(SOCKET Connection);
public:
    void streamClients();
    void Close();
    ~Echo() {
        if (status) {
            status = false;
            closesocket(sListen);
            WSACleanup();
            Counter = 0;
            std::cout << "Server was stoped. \n";
        }
    }
};

void Echo::Connect() {
    while (status) {
        newConnection = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr);
        if (newConnection == 0 || !status) {
            std::cout << "Disconnet \n";
            closesocket(newConnection);
            break;
        }
        else {
            if (Counter < MaxClients) {
                const char* message = "Welcome to Echo!\n";
                std::cout << "Connect " << Counter << "\n";
                send(newConnection, message, strlen(message), NULL);
                Counter++;
                // CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler,(LPVOID) newConnection, NULL, NULL);
                std::thread StreamClientHandler(&Echo::ClientHandler, this, newConnection);
                StreamClientHandler.detach();
            }
            else {
                std::cout << "Error connection " << "\n";
                const char* message = "Server is full. Try later. Echo. \n \n";
                send(newConnection, message, strlen(message), NULL);
                closesocket(newConnection);
                newConnection = NULL;
            }
        }
    }
}

void Echo::ClientHandler(SOCKET Connection) {
    while (true) {
        char message[1];
        if (recv(Connection, message, 1, NULL) == 0)
            break;
        if (send(Connection, message, 1, NULL) == 0)
            break;
    }
    Counter--;
    std::cout << "Disconect client " << Counter << std::endl;
    closesocket(Connection);
    Connection = NULL;
}

void Echo::streamClients() {
    std::thread stream(&Echo::Connect, this);
    stream.detach();
    std::cout << "Stream is begined \n";
}

void Echo::Close() {
    status = false;
    closesocket(sListen);
    WSACleanup();
    Counter = 0;
    std::cout << "Server was stoped. \n";
}

int main(int argc, char* argv[]) {
    SetLib();
    Echo cons;
    cons.SetValues();
    cons.Start();
    cons.streamClients();
    std::cout << "Any key for stop \n";
    std::cin.ignore();
    std::cin.get();
        cons.Close();
    system("pause");
    return 0;
}

static void SetLib() {
    WSAData wsadata;
    WORD DLLVersion = MAKEWORD(2, 1);
    if (WSAStartup(DLLVersion, &wsadata) != 0)
    {
        std::cout << "Error lib" << std::endl;
        exit(1);
    }
}
