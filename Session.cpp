#include"Session.h"

#include<unistd.h>
#include<cstring>
#include<sys/socket.h>
#include<string>
#include<ctype.h>
#include<iostream>
#include<netinet/in.h>

Session::Session(int ClientFd,const std::string& RootDir)
    :_ClientFd(ClientFd)
    ,_RootDir(RootDir)
    ,_DataFd(-1)
{
}

Session::~Session()
{
    close(_ClientFd);
}

void Session::run()
{
    const char* MessageBegin = "220 FTP server already ready >~<\r\n";
    send(_ClientFd,MessageBegin,strlen(MessageBegin),0);

    //

    char buf[1024];
    int n = 0;
    while((n=recv(_ClientFd,buf,sizeof(buf) - 1,0)) > 0)
    {
        buf[n]='\0';
        
        //

        if(n>=2&&buf[n-1]=='\n'&&buf[n-2]=='\r')
        {
            buf[n-2]='\0';
        }
        else if(n>=1&&buf[n-1]=='\n')
        {
            buf[n-1]='\0';
        }//有的只发\n   (用xxd看没有0d  只有0a：\n)
        printf("客户端:%s\n",buf);
        //const char* response = "502 command not implemented \r\n";
        //send(_ClientFd,response,strlen(response),0);

        //

        char* FindSpace = strchr(buf,' ');
        char* cmd = buf;
        char* num = nullptr;
        if(FindSpace != nullptr)
        {
            *FindSpace = '\0';
            num = FindSpace + 1;
        }
        for(;*cmd != '\0';cmd++)
        {
            *cmd = toupper(*cmd);
        }
        if(strcmp(buf,"QUIT") == 0)
        {
            const char* bye = "221 ByeBye >~<\r\n";
            send(_ClientFd,bye,strlen(bye),0);
            close(_ClientFd);
            break;
        }
        else if(strcmp(buf,"PWD")==0)
        {
            char cwd[1024];
            getcwd(cwd,sizeof(cwd));
            char MessagePWD[1200];
            snprintf(MessagePWD,sizeof(MessagePWD),"257 NowPath is that %s\r\n",cwd);
            send(_ClientFd,MessagePWD,strlen(MessagePWD),0);
        }

        //

        else if(strcmp(buf,"USER")==0)
        {
            _Username = num?num:"";
            const char* response = "331 Password required:\r\n";
            send(_ClientFd,response,strlen(response),0);
        }
        else if(strcmp(buf,"PASS")==0)
        {
            if(num && strcmp(num,"20260501")==0)
            {
                _WhetherLogin = true;
                const char* PasswordGreat = "230 Your Password is perfect!now log in successfully!\r\n";
                send(_ClientFd,PasswordGreat,strlen(PasswordGreat),0);
            }
            else
            {
                const char* Wrong = "530 Sorry,Please try again!\r\n";
                send(_ClientFd,Wrong,strlen(Wrong),0);
            }   
        }
        else if(strcmp(buf,"CWD")==0)
        {
            if(chdir(num)==0)
            {
                const char* message2 = "250 changed now directory successfully!\r\n";
                send(_ClientFd,message2,strlen(message2),0);
            }
            else
            {
                const char* message2 = "550 Failed to change the directory!\r\n";
                send(_ClientFd,message2,strlen(message2),0);
            }            
        }
        else if(strcmp(buf,"TYPE")==0)
        {
            if(num && strcmp(num,"I")==0)
            {
                const char* message3 = "200 Type set to I\r\n";
                send(_ClientFd,message3,strlen(message3),0);
            }
            else
            {
                const char* message3 = "504 Type not supported\r\n";
                send(_ClientFd,message3,strlen(message3),0);
            }
        }
        
        else if(strcmp(buf,"PASV")==0)
        {
            _DataFd = socket(AF_INET,SOCK_STREAM,0);
            if(_DataFd < 0)
            {
                std::cerr << "_DataFd socket boom!!!" << std::strerror(errno)<< std::endl;
            }

            //

            struct sockaddr_in DataAddr;
            memset(&DataAddr,0,sizeof(DataAddr));
            DataAddr.sin_family = AF_INET;
            DataAddr.sin_port = htons(0);
            DataAddr.sin_addr.s_addr = htonl(INADDR_ANY);

            //

            int bindRet = bind(_DataFd,(sockaddr*)&DataAddr,sizeof(DataAddr));
            if(bindRet < 0)
            {
                std::cerr << "DataFd bind boom!!!!!!!!" << std::strerror(errno) << std::endl;
                return;
            }

            //

            int listenRet = listen(_DataFd,1);//  0不排队，1排队 
            if(listenRet < 0)
            {
                std::cerr << "DataFd listen boom!!!!!!!!" << std::strerror(errno) << std::endl;
            }
            
            //
            
            socklen_t AddrLen = sizeof(DataAddr);
            getsockname(_DataFd,(sockaddr*)&DataAddr,(socklen_t*)&AddrLen);
            
            
             




        }


        
        //
        
        else
        {
            const char* message = "502 Not-found!!!\r\n";
            send(_ClientFd,message,strlen(message),0);
        }

    }









}

