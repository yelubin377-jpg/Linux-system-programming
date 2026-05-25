#pragma once
#include<string>

class Session
{
public:
    Session(int ClientFd,const std::string& RootDir);
    ~Session();
    void run();
private:
    int _ClientFd;
    std::string _RootDir;
};