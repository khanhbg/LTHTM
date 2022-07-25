#include <iostream>
#include "httpcli.h"
#include "cstring"
#include <string>
#include "httptcpclient.h"
#include <stdio.h>
#include <vector>
HttpCLI::HttpCLI():CmdLineInterface("Http>")
{}
void HttpCLI::initCmd()
{
    addCmd("head",CLI_CAST(&HttpCLI::doHead));
    addCmd("get",CLI_CAST(&HttpCLI::doGet));
    addCmd("gets",CLI_CAST(&HttpCLI::doGets));
    //addCmd("open",CLI_CAST(&HttpCLI::doOpen));
    addCmd("help", CLI_CAST(&HttpCLI::doHelp));
    addCmd("close", CLI_CAST(&HttpCLI::doClose));
}
void HttpCLI::doHead(char * cmd_argv[], int cmd_argc)
{
    if(cmd_argc == 4) //head /vi/don-vi/khoa-kinh-te-quan-ly-xay-dung.html www.nuce.edu.vn 80
    {
        if(client.isConnected())
            cout << "Already connected. Close connection first" << endl;
        else
        {
            bool ok = client.open(cmd_argv[2],atoi(cmd_argv[3]));
            if(ok)
            {
                cout << "Connected." << endl;
                char newCmd[256];
                sprintf(newCmd,"HEAD %s HTTP/1.0\r\nHost:%s\r\nConnection: close\r\n\r\n",cmd_argv[1],cmd_argv[2]);
                cout<<newCmd;
                client.head(newCmd);
            }
            else
            {
                cout<<"Not connected"<<endl;
            }
        }
    }
    else
    {
        cerr << "cu phap: head + url + host + port";
    }
}

void HttpCLI::doGet(char * cmd_argv[], int cmd_argc) //get urrl host filename
{
    if(cmd_argc==4)
    {
        if(atoi(cmd_argv[3])!=80)
        {
            if(client.isConnected())
                cout << "Already connected. Close connection first" << endl;
            else
            {
                bool ok = client.open(cmd_argv[2],80);
                if(ok)
                {
                    cout << "Connected." << endl;
                    char newCmd[256];
                    sprintf(newCmd,"GET %s HTTP/1.0\r\nHost:%s\r\nConnection: close\r\n\r\n",cmd_argv[1],cmd_argv[2]); //dinh dang request GET URL(/vi/don-vi/khoa-kinh-te-quan-ly-xay-dung.html HTTP/1.0
                    cout<<"REQUEST: "<<endl<<newCmd;
                    //Host (www.nuce.edu.vn)
                    client.get(newCmd,cmd_argv[3]);
                }
                else
                {
                    cout<<"Not connected"<<endl;
                }                                                               //Connection: close
            }
        }
    }
    else if(cmd_argc == 5)    //get 1 / www.nuce.edu.vn filename
    {
        if(atoi(cmd_argv[1])==1)
        {
            if(client.isConnected())
                cout << "Already connected. Close connection first" << endl;
            else
            {
                bool ok = client.open(cmd_argv[3],80);
                if(ok)
                {
                    vector<string> k;
                    vector<string> h;
                    string url;//link
                    string name;//ten file
                    string response;
                    string u;
                    int i,j,l;
                    cout << "Connected." << endl;
                    char newCmd[256];
                    sprintf(newCmd,"GET %s HTTP/1.0\r\nHost:%s\r\nConnection: close\r\n\r\n",cmd_argv[2],cmd_argv[3]); //dinh dang request GET URL(/vi/don-vi/khoa-kinh-te-quan-ly-xay-dung.html HTTP/1.0
                    cout<<"REQUEST: "<<endl<<newCmd;
                    response= client.getf(newCmd,cmd_argv[4]); //goi ham getf voi tham so la ban tin request va ten file
                    //tìm cac HTML
                    i=response.find("<a href=\"/");
                    if(i!=-1)
                    {
                        u = response.substr(i+9); //gan u cho từ vị tri i den het
                        j=u.find("</a>"); //tim vi tri  de lay chuoi con
                        url= u.substr(0,j+4);
                        if (url.find(".html",0)!=-1&&url.find("<img",0)==-1)
                        {
                            k.push_back(name);   //neu la url .html thi nhet vao k
                            //cout<<url<<endl;
                        }
                        while(i!=-1)
                        {
                            i=u.find("<a href=\"/",0); //tim vi tri de cat
                            if(i==-1) //neu k tim duoc thoat khoi vong lap
                            {
                                break;
                            }

                            u=u.substr(i+9);    //chuoi con lai sai khi bo di url dau tien
                            j=u.find("</a>",0);
                            url=u.substr(0,j+4);
                            if (url.find(".html",0)!=-1&&url.find("<img",0)==-1)
                            {
                                k.push_back(url);
                                //cout<<url<<endl;
                            }

                        }
                    }
                    while(k.size()>0)
                    {
                        cout<<k.size()<<endl;
                        u= k.back();
                        k.pop_back();
                        i= u.find("\"",0);
                        if(i!=-1)
                        {
                            url= u.substr(0,i); //link
                            //cout<< url<<endl;
                            i= u.find(">",i+1);
                            if(i!=-1)
                            {
                                j= u.find("</a>",i+1);
                                if(j!=-1)
                                {
                                    name =u.substr(i+1,j-i-1);
                                    //cout<< name<<endl; //name
                                    char fname[1024];
                                    sprintf(fname,"%s  %s",url.c_str(),name.c_str());
                                    //cout<< fname<<endl;
                                    string fn(fname);
                                    cout<<fn<<endl;
                                    h.push_back(fn);
                                    //cout<< url<<endl;
                                }
                            }
                        }
                    }
                    //TIM IMG
                    i=response.find("src=\"");
                    if(i!=-1)
                    {
                        u = response.substr(i+5); //gan u cho từ vị tri i den het
                        j=u.find("\"",0); //vi tri cua 1 trong u
                        url= u.substr(0,j);
                        if (url.find(".png",0)!=-1||url.find(".jpeg",0)!=-1||url.find(".gif",0)!=-1)
                        {
                            k.push_back(name);
                            //cout<<url<<endl;
                        }
                        while(i!=-1)
                        {
                            i=u.find("src=\"",0);
                            if(i==-1)
                            {
                                break;
                            }

                            u=u.substr(i+5);
                            j=u.find("\"",0);
                            url=u.substr(0,j);
                            if (url.find(".png",0)!=-1||url.find(".jpeg",0)!=-1||url.find(".gif",0)!=-1)
                            {
                                k.push_back(url);
                                //cout<<url<<endl;
                            }
                        }
                    }
                    while(k.size()>0)
                    {
                        u= k.back();
                        url=u;
                        //cout<<url<<endl;
                        k.pop_back();
                        i= url.rfind("/");
                        if(i!=-1)
                        {
                            j=u.rfind(".");
                            if(j!=-1)
                            {
                                name =url.substr(i+1,j-i-1);
                                //cout<< name<<endl; //name
                                char fname[1024];
                                sprintf(fname,"%s  %s",url.c_str(),name.c_str());
                                //cout<< fname<<endl;
                                string fn(fname);
                                //cout<<fn<<endl;
                                h.push_back(fn);
                            }
                        }
                    }
                    while(h.size()>0)
                    {
                        u=h.back();
                        h.pop_back();
                        i=u.find("  ");
                        url=u.substr(0,i);
                        cout<<url<<endl;
                        name=u.substr(i+2);
                        cout<<name<<endl;

                        bool ok = client.open(cmd_argv[3],80);
                        if(ok)
                        {
                            cout << "Connected." << endl;
                            sprintf(newCmd,"GET %s HTTP/1.0\r\nHost:%s\r\nConnection: close\r\n\r\n",url.c_str(),cmd_argv[3]);
                            cout<<newCmd;
                            client.get(newCmd,name.c_str());
                            cout<<"||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||"<<endl;
                        }
                        else
                        {
                            cout<<"loiiiii";
                        }
                    }


                    cout<<"----------NGUY----------"<<endl;

                }


                else
                {
                    cout<<"Not connected"<<endl;
                }                                                               //Connection: close
            }
        }
        else

            cerr<<"Sai cu phap"<<endl;
    }
    else
    {
        cerr<<"sai cu phap, go help de tro giup"<<endl;
    }
}

void HttpCLI::doGets(char * cmd_argv[], int cmd_argc)
{
    if(cmd_argc == 2)
    {
        char filename[256];
        if(strcmp(cmd_argv[1],"kb")== 0)   //if(=o){for nhap url,for tach url      }
        {
            vector<string> v;
            string u;
            int i=0;
            do
            {
                cout<<"TAI NGUYEN "<<i<<" = ";
                i++;
                getline(cin,u);
                if(u.length()>0)
                {
                    v.push_back(u);
                }
            }
            while(u.length()>0);
            while(v.size()>0) //tach goi client.get url + host +port + file
            {

                char *file;
                char *host;
                char *url;
                //cout<<v.back()<<endl;
                const char *cstr=v.back().c_str(); //chuyen string v.back() sang char[] tra ve const
                //cout<< cstr<<endl;
                char *st=new char[strlen(cstr)+1]; // tao char st moi co size bang size csrt+1
                strcpy(st,cstr); //copy cstr sang st
                //cout<<st;
                url = strtok(st," "); //tach
                cout<< url;
                if(url)
                {
                    host = strtok(NULL," ");
                    if(host)
                    {
                        file = strtok(NULL," ");
                    }

                }
                bool ok = client.open(host,80);
                if(ok)
                {
                    cout << "Connected." << endl;
                    char newCmd[256];
                    sprintf(newCmd,"GET %s HTTP/1.0\r\nHost:%s\r\nConnection: close\r\n\r\n",url,host);
                    cout<<newCmd;
                    client.get(newCmd,file);
                    cout<<"||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||"<<endl;
                }
                else
                {
                    cout<<"loiiiii";
                }
                v.pop_back();
            }
        }
    }
    else if (strcmp(cmd_argv[1],"fn")==0) //doc file
    {
        char link[256];
        cout<<"Xin moi nhap ten file"<<endl; //"F:/WEB_GET/k.txt/"
        cin.getline(link,256);
        FILE *f;
        f=fopen(link,"r");
        if(f!=NULL)
        {
            char buf[256];
            char* url;
            char*file;
            char *host;
            while(!feof(f)) //doc file cho den khi eof
            {
                fgets(buf,256,f);
                cout<<buf<<endl;
                //puts(bu);
                url = strtok(buf," ");  //tach lay url
                cout<< url<<endl;
                if(url)
                {
                    host = strtok(NULL," ");
                    cout<< host<<endl;
                    if(host)
                    {
                        file = strtok(NULL,"\n");
                        cout<<file<<endl;
                    }
                }

                bool ok = client.open(host,80);
                if(ok)
                {
                    cout << "Connected." << endl;
                    char newCmd[256];
                    sprintf(newCmd,"GET %s HTTP/1.0\r\nHost:%s\r\nConnection: close\r\n\r\n",url,host);
                    cout<<"REQUEST: "<<endl<<newCmd;
                    client.get(newCmd,file);
                    cout<<"||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||"<<endl;
                }
                else
                    cerr<<"Khong the ket noi den server";
            }
        }

        else
        {
            cerr<<"loi mo file";
        }
    }
    else
    {
        cerr << "Usage: gets \n";
    }
}
void HttpCLI::doClose(char * cmd_argv[], int cmd_argc)
{
    client.close();
}
void HttpCLI::doHelp(char * cmd_argv[], int cmd_argc)
{
    cout << "Cac lenh duoc ho tro:" << endl;
    cout << "- head <url> <server> <port> Kiem tra tinh trang tai nguyen" << endl;
    cout << "- get <url> <server> <filename> Tai tai nguyen" << endl;
    cout << "- get 1 <url> <server> <ten file> Tim va tai tai nguyen" << endl; // vd : get 1 / ww.nuce.edu.vn file
    cout << "- gets fn Tai nhieu tai nguyen doc trong file txt" << endl; //sau do nhap url+host+fliename
    cout << "- gets kb Tai tai nguyen nhap tu ban phim" << endl; // file luu voi dinh dang url host filename
    cout << "- close Dong ket noi" << endl;
    cout << "- help Tro giup" << endl;
}
