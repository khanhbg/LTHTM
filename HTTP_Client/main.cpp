#include <iostream>
#include "httpcli.h"
using namespace std;

int main()
{
    cout << "Hello world!" << endl;
    HttpCLI cli;
    cli.initCmd();
    cli.run();


    return 0;
}
