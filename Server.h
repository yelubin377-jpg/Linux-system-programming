#pragma once
#include <string> 
#include <atomic>

#include<iostream>
#include<cstring>
#include<thread>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

class Server
{
public:
    explicit Server(int port,std::string RootDir);
    ~Server();
    void run();
    void stop();
private:
    int _ListenFd;
    int _Port;
    std::string _RootDir;
    std::atomic<bool> _Running{false};


};
