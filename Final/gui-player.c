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
int len;
struct addrinfo *r;
int nread, result;

//string input, response;
static char buffer[128];
int buflen = sizeof(buffer);

char serverMess[100];
char playerNumStr[20];

GtkWidget *window;
GtkWidget *vbox, *hbox;
//GtkWidget *entryS, *entryP;
GtkWidget *button, *view;
GtkWidget *text;

static void ready( GtkWidget *widget )
{  
    if(!inGame) {
     write(sockfd, "READY", 6);
     nread = read(sockfd, buffer, buflen);
     //out <<""<< buffer << endl;
     printf("%s\n", buffer); 
     inGame=1;
    }
}

static void pick ( GtkWidget *widget );
/*{
    if(inGame) {
      strcpy(serverMess, "");
      char c[15];// = gtk_button_get_label (GTK_BUTTON(widget));
      strcpy(c, gtk_button_get_label (GTK_BUTTON(widget)) );
      printf("%s\n", c );
      write(sockfd, c, strlen(c)+1);
      nread = read(sockfd, serverMess, buflen);

      gtk_text_set_editable(GTK_TEXT(text), TRUE);
      gtk_editable_insert_text (GTK_EDITABLE (text), serverMess, -1, 0);
      gtk_editable_select_region (GTK_EDITABLE (text),
                                  0, GTK_ENTRY (text)->text_length);
      gtk_text_set_editable(GTK_TEXT(text), FALSE);
     
     inGame = TRUE;
    }
}*/

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

    gtk_box_pack_start (GTK_BOX (vbox), button, FALSE, FALSE, 0);

    // Rock, Paper, Scissors
    gtk_box_pack_start(GTK_BOX(vbox), hbox, TRUE, TRUE, 0);

    // Rock
    button = gtk_button_new_with_label ("Rock");
    g_signal_connect (button, "clicked", G_CALLBACK(pick), NULL);
    gtk_box_pack_start (GTK_BOX (hbox), button, TRUE, TRUE, 0);

    // Paper
    button = gtk_button_new_with_label ("Paper");
    g_signal_connect (button, "clicked", G_CALLBACK(pick), NULL);
    gtk_box_pack_start (GTK_BOX (hbox), button, TRUE, TRUE, 0);

    // Scissors
    button = gtk_button_new_with_label ("Scissors");
    g_signal_connect (button, "clicked", G_CALLBACK(pick), NULL);
    gtk_box_pack_start (GTK_BOX (hbox), button, TRUE, TRUE, 0);

    // Game Status
    //view = gtk_label_new(serverMess);
    //gtk_box_pack_start (GTK_BOX (vbox), view, FALSE, TRUE, 0);
    text = gtk_text_new (NULL, NULL);
    //gtk_table_attach (GTK_TABLE (table), text, 0, 1, 0, 1,
    //                GTK_EXPAND | GTK_SHRINK | GTK_FILL,
    //                GTK_EXPAND | GTK_SHRINK | GTK_FILL, 0, 0);

    gtk_entry_set_text (GTK_ENTRY (text), serverMess);
    gtk_text_set_editable (GTK_TEXT (text), FALSE);
    //gtk_editable_insert_text (GTK_EDITABLE (text), serverMess, -1, 0);
    //gtk_editable_select_region (GTK_EDITABLE (text),
    //                            0, GTK_ENTRY (text)->text_length);
    gtk_text_set_editable(GTK_TEXT(text), FALSE);
    gtk_box_pack_start (GTK_BOX (vbox), text, TRUE, TRUE, 0);


    // Quit
    button = gtk_button_new_with_label ("QUIT");
    //g_signal_connect (button, "clicked", G_CALLBACK(ready), NULL);
    g_signal_connect_swapped (button, "clicked",
                            G_CALLBACK (gtk_widget_destroy), window);
    gtk_box_pack_start (GTK_BOX (vbox), button, FALSE, FALSE, 0);






    ///
    gtk_widget_show_all (window);
    //gtk_widget_show (window);   

    gtk_main();
    
    return 0;
}


static void pick ( GtkWidget *widget )
{
    if(inGame) {
      strcpy(serverMess, "");
      char c[15];// = gtk_button_get_label (GTK_BUTTON(widget));
      strcpy(c, gtk_button_get_label (GTK_BUTTON(widget)) );
      printf("%s\n", c );
      write(sockfd, c, strlen(c)+1);
      nread = read(sockfd, serverMess, buflen);

      gtk_text_set_editable(GTK_TEXT(text), TRUE);
      gtk_editable_insert_text (GTK_EDITABLE (text), serverMess, -1, 0);
      //gtk_editable_select_region (GTK_EDITABLE (text),
      //                            0, GTK_ENTRY (text)->text_length);
      gtk_text_set_editable(GTK_TEXT(text), FALSE);
    
      inGame = TRUE;
    }
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
    nread = read(sockfd, buffer, buflen);
    printf("%s\n", buffer);
    strcpy(playerNumStr, buffer);
}

void newWindow()
{
    // Create new window
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_widget_set_size_request (GTK_WIDGET (window), 400, 200);
    gtk_window_set_title (GTK_WINDOW (window), "DN Fun");
    g_signal_connect (window, "destroy",
                      G_CALLBACK (gtk_main_quit), NULL);
    g_signal_connect_swapped (window, "delete-event",
                              G_CALLBACK (gtk_widget_destroy),
                              window);

    vbox = gtk_vbox_new (FALSE, 0);
    hbox = gtk_hbox_new (FALSE, 0);

    gtk_container_add (GTK_CONTAINER (window), vbox);
}


