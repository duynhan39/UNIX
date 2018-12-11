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
int inGame=-1;

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
GtkWidget *entryS, *entryP;

int connectToServer();
void newWindow();

static void enter_info( GtkWidget *widget )
{
    if(inGame == -1) {
 
      server = gtk_entry_get_text (GTK_ENTRY (entryS));
      printf ("Server: %s\n", server);

      port = gtk_entry_get_text (GTK_ENTRY (entryP));
      printf ("Port: %s\n", port);

      inGame = connectToServer();

      gtk_label_set_text(GTK_LABEL(view), playerNumStr);
    }
}

static void ready( GtkWidget *widget )
{  
    if(inGame == 0) {
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

static void stop ( GtkWidget *widget )
{
    if(inGame==1) {
        write(sockfd, "STOP", 5);
        
        nread = read(sockfd, serverMess, bufflen);
        strcat(serverMess, "\nYou are now out!");
        gtk_label_set_text(GTK_LABEL(text), serverMess);
        printf("%s\n\n", serverMess);
        
        inGame = -1;
        close(sockfd);
    }
}

static void quit ( GtkWidget *widget )
{
   
    if(inGame == 1)
        stop(widget);
      
    gtk_main_quit();
}

int main( int argc, char *argv[] )
{
    gtk_init (&argc, &argv);

    
    // Create new window
    newWindow();

    
    // Server
    hbox = gtk_hbox_new (FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, TRUE, TRUE, 0);

    char mess[50] = "Server:";
    view = gtk_label_new(mess);
    gtk_box_pack_start (GTK_BOX (hbox), view, TRUE, FALSE, 0);

    entryS = gtk_entry_new_with_max_length(50);    
    gtk_box_pack_start (GTK_BOX (hbox), entryS, TRUE, FALSE, 0); 
    

    // Port
    hbox = gtk_hbox_new (FALSE, 0); 
    gtk_box_pack_start(GTK_BOX(vbox), hbox, TRUE, TRUE, 0);

    strcpy(mess, "Port:");
    view = gtk_label_new(mess);
    gtk_box_pack_start (GTK_BOX (hbox), view, TRUE, FALSE, 0);
   
    entryP = gtk_entry_new_with_max_length(50);
    gtk_box_pack_start (GTK_BOX (hbox), entryP, TRUE, FALSE, 0);


    // Connect
    button = gtk_button_new_with_label ("Connect");
    g_signal_connect (button, "clicked",
                            G_CALLBACK (enter_info), NULL);
    gtk_box_pack_start (GTK_BOX (vbox), button, TRUE, TRUE, 0);

 
    // Player Number
    view = gtk_label_new(playerNumStr);
    gtk_box_pack_start (GTK_BOX (vbox), view, FALSE,FALSE, 0);

    
    // Ready
    button = gtk_button_new_with_label ("READY");
    g_signal_connect (button, "clicked", G_CALLBACK(ready), NULL);
    
    gtk_box_pack_start (GTK_BOX (vbox), button, TRUE, FALSE, 0);

    
    // Rock, Paper, Scissors
    hbox = gtk_hbox_new (FALSE, 0);
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
    button = gtk_button_new_with_label ("STOP");
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

int connectToServer()
{
    // Connecting to the server
    getaddrinfo(server, port, NULL, &r);
    sockfd = socket(r->ai_family, r->ai_socktype, r->ai_protocol);
    result = connect(sockfd, r->ai_addr, r->ai_addrlen);
    if(result == -1) {
        printf("%s", "client: connect failed\n");
	return -1;
    }
    nread = read(sockfd, buffer, bufflen);
    printf("%s\n", buffer);
    strcpy(playerNumStr, buffer);
    return 0;
}

void newWindow()
{
    // Create new window
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_widget_set_size_request (GTK_WIDGET (window), 400, 400);
    gtk_window_set_title (GTK_WINDOW (window), "DN Fun");
    
    g_signal_connect (window, "destroy",
                      G_CALLBACK (quit), NULL);
   
    vbox = gtk_vbox_new (FALSE, 0);
    gtk_container_add (GTK_CONTAINER (window), vbox);
}


