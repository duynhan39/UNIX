//  Make the necessary includes and set up the variables. 

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <fstream>

using namespace std;

int main(int argc, char *argv[])
{
    int sockfd;
    int len;
    struct addrinfo *r;
    int nread, result;

    if(argc != 3) {
       cerr << "syntax: slt_client server port" << endl;
       exit(1);
    }

    // memset(&h, 0, sizeof(h));
    // h.ai_flags = AI_NUMERICHOST | AI_NUMERICSERV;
    // getaddrinfo(argv[1], argv[2], &h, &r);
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

    string inf, outf;
    cout<<"Enter input and output file names: ";
    
    while(cin>>inf) {
      
      cin>>outf;
    
      ifstream inFile(inf);
      ofstream outFile(outf);

      while(!inFile)
      {
        cout<<"Error opening input file "<< inf <<endl;
        cout<<"Please input another name: ";
        cin>>inf; 
        inFile.open(inf);   
      }
      while(!outFile)
      {
        cout<<"Error opening output file "<< outf <<endl;
        cout<<"Please input another name: ";
        cin>>outf;
        outFile.open(outf);
      }

      while(getline(inFile, input))
      {
        write(sockfd, input.c_str(), input.length()+1);
        // This should be a read loop
        nread = read(sockfd, buffer, buflen);
        outFile << buffer << endl;
      }
      inFile.close();
      outFile.close();
      cout<<"Conversion is complete."<<endl;
      cout<<"Enter input and output file names: ";
    }
    cout<<endl;
    close(sockfd);
    exit(0);
}
