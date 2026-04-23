#ifndef FD_H
#define FD_H
#include <iostream>
#include <thread>
#include <unistd.h>
#include <cstdio>
#include <cstring>

#include <utility>
#include <sys/socket.h>
#include <netinet/in.h>

class Fd
{
private:
    int fd_;

public:
    explicit Fd(int fd) : fd_(fd) {};
    ~Fd(){if(fd_ >= 0) close(fd_);}

    Fd(const Fd&) = delete;
    Fd& operator = (const Fd&) = delete;

    Fd(Fd&&other)noexcept : fd_(std::exchange(other.fd_,-1)) {}
    Fd& operator = (Fd&&other) noexcept
    {
        std::swap(fd_,other.fd_);
        return *this;
    }

    int get() const {return fd_;}
         
}






















#endif