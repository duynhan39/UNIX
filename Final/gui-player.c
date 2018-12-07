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


GtkWidget *window;
GtkWidget *vbox, *hbox;
//GtkWidget *entryS, *entryP;
GtkWidget *button, *view;
GtkWidget *check;

/*
static void enter_server( GtkWidget *widget,
                           GtkWidget *entry)
{
    server = gtk_entry_get_text (GTK_ENTRY (entry));
    printf ("Server: %s\n", server);
}


static void enter_port( GtkWidget *widget,
                           GtkWidget *entry)
{
    port = gtk_entry_get_text (GTK_ENTRY (entry));
    printf ("Port: %s\n", port);
}
*/

static void enter_info( GtkWidget *widget,
			GtkWidget *entryS,
			GtkWidget *entryP)
{  
    if(!inGame) { 
     // do {
      server = gtk_entry_get_text (GTK_ENTRY (entryS));
      printf ("Server: %s\n", server);

      port = gtk_entry_get_text (GTK_ENTRY (entryP));
      printf ("Port: %s\n", port);

      //getaddrinfo(server, port, NULL, &r);
      // Create a socket for the client.
      //sockfd = socket(r->ai_family, r->ai_socktype, r->ai_protocol);
      // Now connect our socket to the server's socket.
      //result = connect(sockfd, r->ai_addr, r->ai_addrlen);
      //  if(result == -1) {
          //cerr << "client: connect failed" << endl;
      //    exit(1);
      //  }
      //} while(1);
    
//    gtk_container_remove(GTK_CONTAINER(vbox), entryS);
//    gtk_container_remove(GTK_CONTAINER(vbox), entryP);
//    gtk_widget_destroy(entryS);
//    gtk_widget_destroy(entryP);

      inGame=1;
    }
    //gtk_widget_destroy(widget);
}

int main( int   argc,
         char *argv[] )
{
    gtk_init (&argc, &argv);
    char *info[2];
    //strcpy(info[0], argv[1]);
    //strcpy(info[1], argv[2]);
    //strcpy(info[0], "LLA");
    //strcpy(info[1], "KSM");
    if(argc != 3) {
      printf("%s","syntax: shout [server] [port]\n");
      exit(1);
    }

    getaddrinfo(argv[1], argv[2], NULL, &r);
    // Create a socket for the client.
    sockfd = socket(r->ai_family, r->ai_socktype, r->ai_protocol);
    // Now connect our socket to the server's socket.
    result = connect(sockfd, r->ai_addr, r->ai_addrlen);
    if(result == -1) {
        printf("%s", "client: connect failed\n");
        exit(1);
    }
    
//    GtkWidget *window;
//    GtkWidget *vbox, *hbox;
//    GtkWidget *entryS, *entryP;
//    GtkWidget *button;
//    GtkWidget *check;
//    gint tmp_pos;
    
    //gtk_init (&argc, &argv);
    
    /* create a new window */
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
    //gtk_widget_show (vbox);
    
    /////////////////////
 /*   // Server
    entryS = gtk_entry_new ();
    
    gtk_entry_set_max_length (GTK_ENTRY (entryS), 50);
    //g_signal_connect (entryS, "activate",
    //                  G_CALLBACK (enter_server),
    //                  entryS);
    gtk_entry_set_text (GTK_ENTRY (entryS), argv[1]);
//    tmp_pos = GTK_ENTRY (entry)->text_length;
    //gtk_editable_insert_text (GTK_EDITABLE (entry), " world", -1, &tmp_pos);
    //gtk_editable_select_region (GTK_EDITABLE (entry),
    //                            0, GTK_ENTRY (entry)->text_length);

    gtk_box_pack_start (GTK_BOX (vbox), entryS, TRUE, TRUE, 0);
    //gtk_widget_show (entry);

    // Port
    entryP = gtk_entry_new (); 
        
    gtk_entry_set_max_length (GTK_ENTRY (entryP), 50);
    //g_signal_connect (entryP, "activate",
    // 			G_CALLBACK (enter_port),
    //                  	entryP);
    gtk_entry_set_text (GTK_ENTRY (entryP), argv[2]);
//    tmp_pos = GTK_ENTRY (entry)->text_length;
    gtk_box_pack_start (GTK_BOX (vbox), entryP, TRUE, TRUE, 0);
    //gtk_widget_show (entry);  

    // Connect
    button = gtk_button_new_with_label ("Connect");
    g_signal_connect (button, "clicked",
			    G_CALLBACK (enter_info), entryP);
    gtk_box_pack_start (GTK_BOX (vbox), button, TRUE, TRUE, 0);
    //gtk_widget_show (button);
*/

// NEW

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

    // Ready
    button = gtk_button_new_with_label ("READY");
    //g_signal_connect (button, "clicked", G_CALLBACK(print_hello), NULL);

    gtk_box_pack_start (GTK_BOX (vbox), button, FALSE, FALSE, 0);

    // Rock, Paper, Scissors
    gtk_box_pack_start(GTK_BOX(vbox), hbox, TRUE, TRUE, 0);

    // Quit








    ///
    gtk_widget_show_all (window);
    //gtk_widget_show (window);   

    gtk_main();
    
    return 0;
}

