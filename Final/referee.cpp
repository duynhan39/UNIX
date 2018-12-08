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

void e_error(const char *m);
void ee_error(const char *m, int errcode);
int referee(string p1, string p2);
void scoreBoard(int *score, int *ID, int &quiter, int &maxc, int &playerNum, bool *isReady, bool &isFirst);

fd_set readfds, testfds;

int main()
{
    int s_sfd, c_sfd;
    struct sockaddr s_addr, c_addr;
    socklen_t s_len, c_len;
    struct addrinfo hints, *res;
    
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
    
    // Name the socket
    result = ::bind(s_sfd, res->ai_addr, res->ai_addrlen);
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
    listen(s_sfd, 2);
    
    FD_ZERO(&readfds);
    FD_SET(s_sfd, &readfds);
    
    //  Now wait for clients and requests.
    //  Since we have passed a null pointer as the timeout
    //  parameter, no timeout will occur.
    //  The program will exit and report an error if select
    //  returns a value of less than 1.  */
    int picked = 0;
    int maxc = 4;
    int playerNum = 0;
    
    int playerId[2];
    playerId[0] = -1;
    playerId[1] = -1;
    
    bool isReady[2];
    isReady[0] = false;
    isReady[1] = false;
    
    int score[2];
    score[0] = 0;
    score[1] = 0;
    
    string userPicked[2];
    bool isFirst = true;
    bool isStoping = false;
    
    int quiter=0;
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
                    cout << "Adding client on fd " << c_sfd << endl;
                    cout << "Current number of clients: " << maxc-3 << endl;
                    maxc++;
                    
                    if(playerNum < 2)
                    {
                        // Finding player an empty spot
                        for(int i=0; i<2; i++)
                        {
                            if(playerId[i] == -1) {
                                playerId[i] = c_sfd;
                                
                                cout<<"Player " + to_string(i+1) + " entered."<<endl;
                                
                                input = "You are player " + to_string(i+1);
                                write(playerId[i], input.c_str(), input.length()+1);
                                playerNum++;
                                break;
                            }
                        }
                    }
                    
                    cout << "Number of player(s): " << playerNum << endl;
                    
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
                        
                        // Reading players' input
                        if(buffer[nread-1] == '\0') {
                            input.append(buffer, nread-1);
                            
                            if(fd == playerId[0]){
                                if(!isReady[0]) {
                                    if(input == "READY")
                                        isReady[0] = true;
                                }
                                else {
                                    if(input == "STOP")
                                    {
                                        isStoping = true;
                                        quiter += 1;
                                        picked++;
                                    }
                                    else {
					transform(input.begin(), input.end(), input.begin(), ::tolower);
                                        userPicked[0] = input;
                                        picked++;
                                    }
                                }
                            }
                            else if(fd == playerId[1]) {
                                if(!isReady[1]) {
                                    if(input == "READY")
                                        isReady[1] = true;
                                }
                                else {
                                    if(input == "STOP")
                                    {
                                        isStoping = true;
                                        quiter += 2;
                                        picked++;
                                    }
                                    else {
					transform(input.begin(), input.end(), input.begin(), ::tolower);
                                        userPicked[1] = input;
                                        picked++;
                                    }
                                }
                            }
                            break;
                        }
                        else {
                            input.append(buffer, nread);
                        }
                    }
                    
                    // GAME ON ////////////////////////////////////
                    if(isReady[0] && isReady[1])
                    {
                        if(isFirst)
                        {
                            isFirst = false;
                            input = "GO";
                            write(playerId[0], input.c_str(), input.length()+1);
                            write(playerId[1], input.c_str(), input.length()+1);
                        }
                        else if(picked == 2)
                        {
                            // STOP ///////////////////////////////////
                            if(isStoping){
                                scoreBoard(score, playerId, quiter, maxc, playerNum, isReady, isFirst);
                                isStoping = false;
                                picked = 0;
                            } else {

//                                cout<<"Player 1: "<< userPicked[0] <<endl;
//                                cout<<"Player 2: "<< userPicked[1] <<endl;

                                int outcome = referee(userPicked[0], userPicked[1]);
                                input = "Unknown";
                                
                                if(outcome == 0)
				  {input = "Draw";}
                                else {
                                    input = "Player "+ to_string((3 - outcome)/2) +" wins!";
                                    score[(outcome+2)/2]++;
                                }
//                                cout<< "Output "<<input<<endl;
                                write(playerId[0], input.c_str(), input.length()+1);
                                write(playerId[1], input.c_str(), input.length()+1);
                            }
                            picked = 0;
                        }
                    }
                }
            }
        }
    }
}

int wordToNum(string choice)
{
    if(choice == "rock" || choice == "r") return 0;
    if(choice == "paper" || choice == "p") return 1;
    if(choice == "scissors" || choice == "s") return 2;
    return -1;
}

int referee(string w1, string w2) {
    int p1 = wordToNum(w1);
    int p2 = wordToNum(w2);
    return  (p1 - p2) - ((p1 - p2)/2) * 3 ;
}

void scoreBoard(int *score, int* ID, int &quiter, int &maxc, int &playerNum, bool *isReady, bool &isFirst) {
    string out = "Final Score\nPlayer 1: " + to_string(score[0])
		+ "\nPlayer 2: "+ to_string(score[1])
		+ "\n\nYou both did great,\nit's all about having fun!\n";
    
    if(quiter == 3) {
        out = "\nGAME OVER!\n" + out;
        write(ID[0], out.c_str(), out.length()+1);
        write(ID[1], out.c_str(), out.length()+1);
        
        close(ID[0]);
        FD_CLR(ID[0], &readfds);
        cout << "removing client on fd " << ID[0] << endl;
        
        close(ID[1]);
        FD_CLR(ID[1], &readfds);
        cout << "removing client on fd " << ID[1] << endl;
        
        maxc-=2;
        playerNum-=2;
        ID[0] = -1;
        ID[1] = -1;
        
    } else {
        quiter--;
        
        // write for the other program
        string quit = "\nGAME OVER!\nPlayer "+to_string(quiter+1)+" quited, game ended!\n"+out;
        write(ID[1 - quiter], quit.c_str(), quit.length()+1);
        
        // write to quiter
        out = "\nGAME OVER!\n" + out;
        write(ID[quiter], out.c_str(), out.length()+1);
        
        close(ID[quiter]);
        FD_CLR(ID[quiter], &readfds);
        cout << "removing client on fd " << ID[quiter] << endl;
        
        maxc--;
        playerNum--;
        ID[quiter] = -1;
    }
    
    score[0] = score[1] = 0;
    
    isReady[0] = isReady[1] = false;
    isFirst = true;
    quiter = 0;
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
