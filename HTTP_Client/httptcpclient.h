#ifndef HttpTcpClient_H_INCLUDED
#define HttpTcpClient_H_INCLUDED
#include "tcpclient.h"
class HttpTcpClient: public TcpClient
{

public:
    char filename[256];
    FILE *f;
    //viet cac ham chua co o day
    HttpTcpClient();
    void head(const string& msg);
    void get(const string& msg,const char* msg2);
    string getf(const string& msg,const char* msg2);

};

#endif // HttpTcpClient_H_INCLUDED
