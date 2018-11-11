#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <netdb.h>
#include <algorithm>

using namespace std;
void shout(string &in);
void e_error(const char *m);
void ee_error(const char *m, int errcode);

int main()
{
    int s_sfd, c_sfd;
    struct sockaddr s_addr, c_addr;
    socklen_t s_len, c_len;
    struct addrinfo hints, *res;
    
    fd_set readfds, testfds;
    static char buffer[128];
    int buflen = sizeof(buffer);
    int result;
    string input;
    char name[1024];
    char port[1024];

    // Use getaddrinfo to get local socket address
    // Use port 0 for dynamic assignment
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    result = getaddrinfo(NULL, "0", &hints, &res);
    if(result == -1)
      e_error("bind");

    // Make a socket
    s_sfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if(s_sfd == -1)
      e_error("socket");
//    cout << "server accepting clients on fd " << s_sfd << endl;
    
    // Name the socket
    result = bind(s_sfd, res->ai_addr, res->ai_addrlen);    
    if(result == -1)
      e_error("bind");

    // Determine assigned port number
    s_len = sizeof(s_addr);
    result = getsockname(s_sfd, &s_addr, &s_len);
    if(result == -1)
      e_error("getsockname");
    
    // Use getnameinfo instead pulling apart address structure.
    // Note that I don't need ntohs and ilk.
    int flags;
    flags = NI_NUMERICHOST | NI_NUMERICSERV;
    result = getnameinfo(&s_addr, sizeof(s_addr),
			 name, sizeof(name), port, sizeof(port), flags);
    if(result != 0)
      ee_error("getnameinfo", result);
    cout << "IP: " << name << "\t\tPORT: " << port << endl;
    freeaddrinfo(res);

    //  Create a connection queue and initialize readfds to handle 
    //  input from server_sockfd.  

    listen(s_sfd, 10);

    FD_ZERO(&readfds);
    FD_SET(s_sfd, &readfds);

    //  Now wait for clients and requests.
    //  Since we have passed a null pointer as the timeout 
    //  parameter, no timeout will occur.
    //  The program will exit and report an error if select 
    //  returns a value of less than 1.  */
    
    int maxc = 4;
    
    while(1) {
        int fd;
        int nread;

        testfds = readfds;

        cout << "server waiting" << endl;
        result = select(maxc, &testfds, (fd_set *)0, 
            (fd_set *)0, (struct timeval *) 0);
        if(result < 1) {
            cerr << "server: error in select call" << endl;
            exit(1);
        }

        //  Once we know we've got activity, we find which descriptor 
        // it's on by checking each in turn using FD_ISSET. 


        for(fd = 0; fd < maxc; fd++) {
            if(FD_ISSET(fd,&testfds)) {

                //  If the activity is on server_sockfd, it must be a 
                //  request for a new connection and we add the associated 
                //  client_sockfd to the descriptor set.  */

                if(fd == s_sfd) {
                    c_len = sizeof(c_addr);
                    c_sfd = accept(s_sfd, &c_addr, &c_len);
                    FD_SET(c_sfd, &readfds);
                    cout << "adding client on fd " << c_sfd << endl;
   		    maxc++;
                }

                //  If it isn't the server, it must be client activity.
                //  If close is received, the client has disconnected and 
                //  we remove it from the descriptor set.
                //  Otherwise, we 'serve' the client

                else {
		  // Read loop
		  input.clear();
		  while(true) {
                    nread = read(fd, buffer, buflen);
                    if(nread == 0) {
                        close(fd);
                        FD_CLR(fd, &readfds);
                        cout << "removing client on fd " << fd << endl;
			maxc--;
			break;
                    }
		    if(buffer[nread-1] == '\0') {
		      input.append(buffer, nread-1);
		      break;
		    }
		    else
		      input.append(buffer, nread);
		  }
		  shout(input);
                  write(fd, input.c_str(), input.length()+1);
                }
            }
        }
    }
}

void shout(string &in) {
  transform(in.begin(), in.end(), in.begin(), ::toupper);
}

void e_error(const char *m)
{
  cerr << m << ": " << strerror(errno) << endl;
  exit(errno);
}

void ee_error(const char *m, int errcode)
{
  cerr << m << ": " << gai_strerror(errcode) << endl;
  exit(errcode);
}

