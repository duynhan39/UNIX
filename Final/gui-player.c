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
static char serverMess[128];
static char buffer[128];
int bufflen = sizeof(serverMess);

//char serverMess[128];
char playerNumStr[20];

GtkWidget *window;
GtkWidget *vbox, *vbox2, *hbox;
//GtkWidget *entryS, *entryP;
GtkWidget *button, *view;
GtkWidget *text;     		// Displaying mess from server

static void ready( GtkWidget *widget )
{  
    if(!inGame) {
     write(sockfd, "READY", 6);
     nread = read(sockfd, serverMess, bufflen);
     //out <<""<< buffer << endl;
     printf("%s\n", serverMess); 
     inGame=1;
     gtk_label_set_text(GTK_LABEL(text), serverMess);
    }
}

static void pick ( GtkWidget *widget )
{
    if(inGame==1) {
      strcpy(serverMess, "");
      char c[15];// = gtk_button_get_label (GTK_BUTTON(widget));
      strcpy(c, gtk_button_get_label (GTK_BUTTON(widget)) );
      printf("%s\n", c );
      write(sockfd, c, strlen(c)+1);
      nread = read(sockfd, serverMess, bufflen);

      gtk_label_set_text(GTK_LABEL(text), serverMess);

      if(serverMess[1] == 'G') {
	inGame = 0; 
      }

      gtk_label_set_text(GTK_LABEL(text), serverMess);
/*
      gtk_text_set_editable(GTK_TEXT(text), TRUE);
      gtk_editable_insert_text (GTK_EDITABLE (text), serverMess, -1, 0);
      gtk_editable_select_region (GTK_EDITABLE (text),
                                  0, GTK_ENTRY (text)->text_length);
      gtk_text_set_editable(GTK_TEXT(text), FALSE);
*/   
   //  inGame = TRUE;
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
//  }
  inGame++;
  close(sockfd);
  }
  //exit(0);
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
    // Rock
    const char *moves[] = {"Rock", "Paper", "Scissors"};

    for(int i=0; i<3; i++) {
    vbox2 = gtk_vbox_new (FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox), vbox2, TRUE, TRUE, 0);
    button = gtk_button_new_with_label (moves[i]);
    g_signal_connect (button, "clicked", G_CALLBACK(pick), NULL);
    gtk_box_pack_start (GTK_BOX (vbox2), button, expanded, filled, 0);
    }
/*    // Paper
    vbox2 = gtk_vbox_new (FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox), vbox2, TRUE, TRUE, 0);
    button = gtk_button_new_with_label ("Paper");
    g_signal_connect (button, "clicked", G_CALLBACK(pick), NULL);
    gtk_box_pack_start (GTK_BOX (vbox2), button, expanded, filled, 0);

    // Scissors
    vbox2 = gtk_vbox_new (FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox), vbox2, TRUE, TRUE, 0);
    button = gtk_button_new_with_label ("Scissors");
    g_signal_connect (button, "clicked", G_CALLBACK(pick), NULL);
    gtk_box_pack_start (GTK_BOX (vbox2), button, expanded, filled, 0);
*/
    // Game Status

    //view = gtk_label_new(serverMess);
    //gtk_box_pack_start (GTK_BOX (vbox), view, FALSE, TRUE, 0);
//    text = gtk_text_new (NULL, NULL);
    //gtk_table_attach (GTK_TABLE (table), text, 0, 1, 0, 1,
    //                GTK_EXPAND | GTK_SHRINK | GTK_FILL,
    //                GTK_EXPAND | GTK_SHRINK | GTK_FILL, 0, 0);

//    gtk_entry_set_text (GTK_ENTRY (text), serverMess);
//    gtk_text_set_editable (GTK_TEXT (text), FALSE);
    //gtk_editable_insert_text (GTK_EDITABLE (text), serverMess, -1, 0);
    //gtk_editable_select_region (GTK_EDITABLE (text),
    //                            0, GTK_ENTRY (text)->text_length);
//    gtk_text_set_editable(GTK_TEXT(text), FALSE);
//    gtk_box_pack_start (GTK_BOX (vbox), text, TRUE, TRUE, 0);


    // Quit
    button = gtk_button_new_with_label ("QUIT");
    g_signal_connect (button, "clicked", G_CALLBACK(stop), NULL);
    //g_signal_connect_swapped (button, "clicked",
    //                        G_CALLBACK (gtk_widget_destroy), window);
    gtk_box_pack_start (GTK_BOX (vbox), button, FALSE, FALSE, 0);

    // Game Status
    text = gtk_label_new(serverMess);
    gtk_box_pack_start (GTK_BOX (vbox), text, TRUE, FALSE, 0);




    ///
    gtk_widget_show_all (window);
    //gtk_widget_show (window);   

    gtk_main();
    
    return 0;
}

/*
static void pick ( GtkWidget *widget )
{
    if(inGame) {
      strcpy(serverMess, "");
      char c[15];// = gtk_button_get_label (GTK_BUTTON(widget));
      strcpy(c, gtk_button_get_label (GTK_BUTTON(widget)) );
      printf("%s\n", c );
      write(sockfd, c, strlen(c)+1);
      nread = read(sockfd, serverMess, buflen);
      printf("%s\n", serverMess);
//      gtk_text_set_editable(GTK_TEXT(text), TRUE);
//      gtk_editable_insert_text (GTK_EDITABLE (text), serverMess, -1, 0);
      //gtk_editable_select_region (GTK_EDITABLE (text),
      //                            0, GTK_ENTRY (text)->text_length);
//      gtk_text_set_editable(GTK_TEXT(text), FALSE);
    
      inGame = TRUE;
    }
}
*/


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
                      G_CALLBACK (gtk_main_quit), NULL);
    g_signal_connect_swapped (window, "delete-event",
                              G_CALLBACK (gtk_widget_destroy),
                              window);

    vbox = gtk_vbox_new (FALSE, 0);
//    vbox2 = gtk_vbox_new (FALSE, 0);
    hbox = gtk_hbox_new (FALSE, 0);

    gtk_container_add (GTK_CONTAINER (window), vbox);
}


