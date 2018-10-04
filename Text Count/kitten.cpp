#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include <iomanip>

using namespace std;

void operate(istream &file, string command);
void noCommand(string command);

int main(int argc, char *argv[])
{
    int opt;
    //char *farg;
    
    opterr = 0;
    
    string command="";
    
    while((opt = getopt(argc, argv, "Ens")) != -1)
    {
        switch(opt)
        {
            case 'E':
                command+="E";
                break;
            case 'n':
                command+="n";
                break;
            case 's':
                command+="s";
                break;
            case '?':
                break;
        }
    }
    
    if(optind >= argc)
        operate(cin, command);

    else
    {
        ifstream file;
        for(; optind < argc; optind++)
        {
            file.open(argv[optind]);
            operate(file, command);
            file.close();
        }
    }
    
    return 0;
}

void operate(istream &file, string command)
{
    string line;
    bool ss = false;
    
    int count=1;
    while(getline(file, line))
    {
        if(command.find('s') != string::npos && line == "")
        {
            if(ss)
                continue;
            else
                ss = true;
        }
        if( line != "" ) ss = false;
        
        if(command.find('n') != string::npos)
        {
            cout<<setw(6)<<count<<" ";
            count++;
        }
        cout<<line;
        
        if(command.find('E') != string::npos)
        {
            cout<<"$";
        }
        
        cout<<endl;
    }
}
