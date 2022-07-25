#ifndef HttpCLI_H_INCLUDED
#define HttpCLI_H_INCLUDED
#include "cli.h"
#include "tcpsocket.h"
#include "httptcpclient.h"
class HttpCLI : public CmdLineInterface
{
private:
    HttpTcpClient client;
public:
    HttpCLI();
    virtual void initCmd();
private:
    //viet cac ham cua chuong trinh vao day
    void doHead(char * cmd_argv[], int cmd_argc);
    void doGet(char * cmd_argv[], int cmd_argc);
    void doGets(char * cmd_argv[], int cmd_argc);
    void doClose(char * cmd_argv[], int cmd_argc);
    void doHelp(char * cmd_argv[], int cmd_argc);
};


#endif // HttpCLI_H_INCLUDED
