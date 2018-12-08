//  Make the necessary includes and set up the variables. 

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

int main(int argc, char *argv[])
{
    const char* moveA[] = {"rock", "paper", "scissors", "r", "p", "s"};
    const vector<string> move(moveA, end(moveA));
    int sockfd;
    int len;
    struct addrinfo *r;
    int nread, result;
    
    if(argc != 3) {
        cerr << "syntax: shout [server] [port]" << endl;
        exit(1);
    }
    
    getaddrinfo(argv[1], argv[2], NULL, &r);
    // Create a socket for the client.
    sockfd = socket(r->ai_family, r->ai_socktype, r->ai_protocol);
    // Now connect our socket to the server's socket.
    result = connect(sockfd, r->ai_addr, r->ai_addrlen);
    if(result == -1) {
        cerr << "client: connect failed" << endl;
        exit(1);
    }
    
    // We can now read/write via sockfd.
    string input, response;
    static char buffer[128];
    int buflen = sizeof(buffer);
    
    nread = read(sockfd, buffer, buflen);
    cout <<""<< buffer << endl;
    
    while(1) {
        cout<<"Say \"READY\" when you are!"<<endl;
        
        input = "";
        while(input != "READY")
        {
            cout<<">> ";
            cin>>input;
        }
        
        write(sockfd, input.c_str(), input.length()+1);
        nread = read(sockfd, buffer, buflen);
        cout <<""<< buffer << endl;
        
        cout<<"\nPlayer pick:\n>> ";
        while(cin>>input)
        {
            if (input == "STOP")
            {
                write(sockfd, input.c_str(), input.length()+1);
                
                nread = read(sockfd, buffer, buflen);
                if (nread > 10) {
                    cout<<buffer<<endl;
                }
                cout << endl;
                close(sockfd);
                exit(0);
            }
            
            transform(input.begin(), input.end(), input.begin(), ::tolower);
            while(find(move.begin(), move.end(), input) == move.end())
            {
                cout<<"Invalid input, please pick one of the options:\n[r]ock\n[p]aper\n[s]cissors\nor \"STOP\" to end the game\n>> ";
                cin>>input;
            }
            
            write(sockfd, input.c_str(), input.length()+1);
            nread = read(sockfd, buffer, buflen);
            cout << buffer << endl;
            if(buffer[1] == 'G') {
                cout<<"\n===========================\n\n";
                break;
            }
            cout<<"\nPlayer pick:\n>> ";
        }
    }
    
    cout << endl;
    close(sockfd);
    exit(0);
}
