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

#include <gtk/gtk.h>

using namespace std;

static void print_hello (GtkWidget *widget,
             gpointer   data)
{
  g_print ("Hello World\n");
}

static void activate (GtkApplication *app,
          gpointer        user_data)
{
  GtkWidget *window;
  GtkWidget *button;
  GtkWidget *button_box;

  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "Window");
  gtk_window_set_default_size (GTK_WINDOW (window), 200, 200);

  button_box = gtk_button_box_new (GTK_ORIENTATION_HORIZONTAL);
  gtk_container_add (GTK_CONTAINER (window), button_box);

  button = gtk_button_new_with_label ("Hello World");
  g_signal_connect (button, "clicked", G_CALLBACK (print_hello), NULL);
  g_signal_connect_swapped (button, "clicked", G_CALLBACK (gtk_widget_destroy), window);
  gtk_container_add (GTK_CONTAINER (button_box), button);

  gtk_widget_show_all (window);
}


int main(int argc, char *argv[])
{
/*
    const char* moveA[] = {"rock", "paper", "scissors", "r", "p", "s"};
    const vector<string> move(moveA, end(moveA));
    int sockfd;
    int len;
    struct addrinfo *r;
    int nread, result;

    cout<<"Flag1"<<endl;
    
    if(argc != 3) {
        cerr << "syntax: shout [server] [port]" << endl;
        exit(1);
    }

    cout<<"Flag2"<<endl;
    
    getaddrinfo(argv[1], argv[2], NULL, &r);
    // Create a socket for the client.
    sockfd = socket(r->ai_family, r->ai_socktype, r->ai_protocol);
    // Now connect our socket to the server's socket.
    result = connect(sockfd, r->ai_addr, r->ai_addrlen);
    if(result == -1) {
        cerr << "client: connect failed" << endl;
        exit(1);
    }

    cout<<"Flag3"<<endl;
*/


    GtkApplication *app;
    int stat;

    app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
    g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
    stat = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);



  
    return stat;


/*  
    // We can now read/write via sockfd.
    // GAME ON ////////////////////////
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
*/

}
