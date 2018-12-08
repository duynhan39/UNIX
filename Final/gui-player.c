#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>

#include <string.h>

char *server;
char *port;
int inGame=0;

int sockfd;
//int len;
struct addrinfo *r;
int nread, result;

static char serverMess[128];
static char buffer[128];
int bufflen = sizeof(serverMess);

char playerNumStr[20];

GtkWidget *window;
GtkWidget *vbox, *vbox2, *hbox;
GtkWidget *button, *view;
GtkWidget *text;     		// Displaying mess from server

static void ready( GtkWidget *widget )
{  
    if(!inGame) {
        write(sockfd, "READY", 6);
        nread = read(sockfd, serverMess, bufflen);
        printf("%s\n", serverMess);
        gtk_label_set_text(GTK_LABEL(text), serverMess);
        inGame=1;
    }
}

static void pick ( GtkWidget *widget )
{
    if(inGame==1) {
        strcpy(serverMess, "");
        char c[15];
        strcpy(c, gtk_button_get_label (GTK_BUTTON(widget)) );
        printf("%s\n", c );
        write(sockfd, c, strlen(c)+1);
        nread = read(sockfd, serverMess, bufflen);
        
        gtk_label_set_text(GTK_LABEL(text), serverMess);
        
        if(serverMess[1] == 'G') {
            inGame = 0;
        }
        
        gtk_label_set_text(GTK_LABEL(text), serverMess);
    }
}

static void quit ( GtkWidget *widget )
{
    if(inGame==1) {
        stop();
        gtk_main_quit();
    }
}

static void stop ( GtkWidget *widget )
{
    if(inGame==1) {
        write(sockfd, "STOP", 5);
        
        nread = read(sockfd, serverMess, bufflen);
        strcat(serverMess, "\nYou are now out!");
        gtk_label_set_text(GTK_LABEL(text), serverMess);
        printf("%s\n\n", serverMess);
        
        inGame++;
        close(sockfd);
    }
}

void connectToServer(char *argv[]);
void newWindow();

int main( int argc, char *argv[] )
{
    gtk_init (&argc, &argv);
    char *info[2];
    if(argc != 3) {
        printf("%s","syntax: shout [server] [port]\n");
        exit(1);
    }
    
    //////////////////////
    connectToServer(argv);
    //////////////////////
    
    // Create new window
    newWindow();
    
    // Server
    char mess[50] = "Server: ";
    strcat(mess, argv[1]);
    //printf("%s\n", mess);
    view = gtk_label_new(mess);
    gtk_box_pack_start (GTK_BOX (vbox), view, FALSE, FALSE, 0);
    
    // Port
    strcpy(mess, "Port: ");
    strcat(mess, argv[2]);
    view = gtk_label_new(mess);
    gtk_box_pack_start (GTK_BOX (vbox), view, FALSE,FALSE, 0);
    
    // Player Number
    view = gtk_label_new(playerNumStr);
    gtk_box_pack_start (GTK_BOX (vbox), view, FALSE,FALSE, 0);
    
    // Ready
    button = gtk_button_new_with_label ("READY");
    g_signal_connect (button, "clicked", G_CALLBACK(ready), NULL);
    
    gtk_box_pack_start (GTK_BOX (vbox), button, TRUE, FALSE, 0);
    
    // Rock, Paper, Scissors
    gtk_box_pack_start(GTK_BOX(vbox), hbox, TRUE, TRUE, 0);
    int expanded = TRUE;
    int filled = FALSE;
    
    const char *moves[] = {"Rock", "Paper", "Scissors"};
    for(int i=0; i<3; i++) {
        vbox2 = gtk_vbox_new (FALSE, 0);
        gtk_box_pack_start(GTK_BOX(hbox), vbox2, TRUE, TRUE, 0);
        button = gtk_button_new_with_label (moves[i]);
        g_signal_connect (button, "clicked", G_CALLBACK(pick), NULL);
        gtk_box_pack_start (GTK_BOX (vbox2), button, expanded, filled, 0);
    }
    
    // Quit
    button = gtk_button_new_with_label ("QUIT");
    g_signal_connect (button, "clicked", G_CALLBACK(stop), NULL);
    
    gtk_box_pack_start (GTK_BOX (vbox), button, FALSE, FALSE, 0);
    
    // Game Status
    text = gtk_label_new(serverMess);
    gtk_box_pack_start (GTK_BOX (vbox), text, TRUE, FALSE, 0);
    
    // Show to screen
    gtk_widget_show_all (window);
    
    gtk_main();
    
    return 0;
}

void connectToServer(char *argv[])
{
    // Connecting to the server
    getaddrinfo(argv[1], argv[2], NULL, &r);
    sockfd = socket(r->ai_family, r->ai_socktype, r->ai_protocol);
    result = connect(sockfd, r->ai_addr, r->ai_addrlen);
    if(result == -1) {
        printf("%s", "client: connect failed\n");
        exit(1);
    }
    nread = read(sockfd, buffer, bufflen);
    printf("%s\n", buffer);
    strcpy(playerNumStr, buffer);
}

void newWindow()
{
    // Create new window
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_widget_set_size_request (GTK_WIDGET (window), 400, 400);
    gtk_window_set_title (GTK_WINDOW (window), "DN Fun");
    
    g_signal_connect (window, "destroy",
                      G_CALLBACK (quit), NULL);
    //                      G_CALLBACK (gtk_main_quit), NULL);
    //    g_signal_connect_swapped (window, "delete-event",
    //                              G_CALLBACK (gtk_widget_destroy),
    //                              window);
    
    vbox = gtk_vbox_new (FALSE, 0);
    hbox = gtk_hbox_new (FALSE, 0);
    gtk_container_add (GTK_CONTAINER (window), vbox);
}


