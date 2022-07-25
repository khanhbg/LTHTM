#include "httptcpclient.h"
#include "httpcli.h"
#include<iostream>
#include<string.h>
#include<stdlib.h>
#include<string>
#include<fstream>
using namespace std;
HttpTcpClient::HttpTcpClient():TcpClient()
{}
void HttpTcpClient::head(const string& message)
{

    int bytes;
    this->sendStringRequest(message); // gửi bản tin
    char buffer[256];
    bytes = this->recvDataBuffer(buffer, 255); // nhận bản tin phản hồi
    while (bytes>0)
    {
        buffer[bytes]=0;
        cout<<buffer;
        bytes = this->recvDataBuffer(buffer, 255); //tiep tuc nhan tin
    }
    if(bytes==0)
    {
        cout<<endl;
        cout<< "Server close";
        cout<<endl;
        close();
    }
}

void HttpTcpClient::get(const string& message,const char * msg2)
{
    int bytes;
    string recv;
    string kq;
    string type;
    char buffer[256];
    char tp[266];
    this->sendStringRequest(message); // gửi bản tin
    bytes = this->recvDataBuffer(buffer,255); // nhận bản tin phản hồi
    strcpy(tp, buffer);
    while (bytes>0)
    {
        //vong lap nhan du lieu
        for(int i=0; i<bytes; i++)
        {
            recv.push_back(buffer[i]);
        }
        buffer[bytes]=0;
        bytes = this->recvDataBuffer(buffer, 255);
    }
    char *ct;
    char *d;
    ct = strstr(tp,"Content-Type:");
    if (ct != NULL)
    {
        d=strtok(ct,"\n");
        //cout<< d;
        if(d!=NULL)
        {
            if(strstr(d,"image/png")!=NULL)
            {
                type=".png";
            }
            else if(strstr(d,"application")!=NULL)
            {
                type=".exe" ;
            }
            else if(strstr(d,"text/html")!=NULL)
            {
                type=".html";
            }
            else if(strstr(d,"jpeg")!=NULL)
            {
                type=".jpeg";
            }
            else if(strstr(d,"gif")!=NULL)
            {
                type=".gif";
            }
            else
                type=".bin";
        }
    }
    fstream f;
    sprintf(filename,"F:/WEB_GET/%s%s",msg2,type.c_str());
    cout<<filename;
    f.open(filename,ios::app|ios::binary); //mo file
    int n = recv.find("\r\n\r\n");
    kq =  recv.substr(n+4);
    f.write(kq.c_str(),kq.size());// luu vao file
    f.close();
    cout<<endl;
    cout<<"TAI VE THANH CONG"<<endl;
    cout<<"Duong dan: "<< filename<<endl;
    close();//dong ket noi
    cout<< "Server close";
    cout<<endl;
}
string HttpTcpClient::getf(const string& message, const char* msg2)
{
    int bytes;
    this->sendStringRequest(message); // gửi bản tin
    string recv;
    string kq;
    string type;
    char buffer[256];
    char tp[266];
    string buf;
    bytes = this->recvDataBuffer(buffer,255); // nhận bản tin phản hồi
    strcpy(tp, buffer);


    while (bytes>0)  //vong lap nhan du lieu
    {
        for(int i=0; i<bytes; i++)
        {
            recv.push_back(buffer[i]);
        }
        buffer[bytes]=0;
        //cout<<buffer;
        bytes = this->recvDataBuffer(buffer, 255);
    }
    char *ct;
    char *d;
    ct = strstr(tp,"Content-Type:");
    if (ct != NULL)
    {
        d=strtok(ct,"\n");
        //cout<< d;
        if(d!=NULL)
        {
            if(strstr(d,"image/png")!=NULL)
            {
                type=".png";
            }
            else if(strstr(d,"application")!=NULL)
            {
                type=".exe" ;
            }
            else if(strstr(d,"text/html")!=NULL)
            {
                type=".html";
            }
            else
                type=".bin";
        }
    }
    fstream f;
    char filename[256];
    sprintf(filename,"F:/WEB_GET/%s%s",msg2,type.c_str());
    cout<<filename;
    f.open(filename,ios::app|ios::binary); //mo file
    int n = recv.find("\r\n\r\n");
    kq =  recv.substr(n+4);
    f.write(kq.c_str(),kq.size());// luu vao file
    f.close();
    cout<<endl;
    cout<<"TAI VE THANH CONG"<<endl;
    cout<<"Duong dan: "<< filename<<endl;
    close();//dong ket noi
    cout<< "Server close";
    cout<<endl;
    cout<<endl;
    return recv;

//cout<<"hihi"<<endl;
}








