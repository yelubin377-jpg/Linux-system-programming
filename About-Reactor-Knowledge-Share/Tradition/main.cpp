#include <iostream>
#include <thread>
#include <unistd.h>
#include <cstdio>
#include <cstring>

#include <sys/socket.h>
#include <netinet/in.h>

void handle_client(int client_fd)
{
    char buf[1024];
    while(read(client_fd,buf,sizeof(buf)))
    {
        write(client_fd,buf,strlen(buf));
    }
    close(client_fd);
}

int main()
{
    int server_fd = socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(8080);
    bind(server_fd,(struct sockaddr*)&addr,sizeof(addr));
    listen(server_fd,10);
    while(true)
    {
        int client_fd = accept(server_fd,nullptr,nullptr);
        std::thread(handle_client,client_fd).detach();
    }
}

