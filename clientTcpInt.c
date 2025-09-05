#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define PORT 3000
#define MAX_BUFFER_SIZE 1224

GtkWidget *result_drawing_area;
GtkWidget *main_window;
int clientSocket;
GtkWidget *input_entry ;
// Fonction pour envoyer des requêtes au serveur
void sendRequest(const char *request) {
    send(clientSocket, request, strlen(request), 0);
}

// Callback pour le bouton "Authentifier"
void authenticate_button_clicked(GtkWidget *widget, gpointer data) {
    const char *username = "eya";
    const char *password = "123";

    // Envoie le nom d'utilisateur et le mot de passe au serveur pour l'authentification
    send(clientSocket, username, strlen(username), 0);
    usleep(100000);
    send(clientSocket, password, strlen(password), 0);

    // Recevoir et afficher le résultat d'authentification du serveur
    char authResult[MAX_BUFFER_SIZE];
    recv(clientSocket, authResult, sizeof(authResult), 0);

    gtk_label_set_text(GTK_LABEL(result_drawing_area), "Authentification réussie.");
    create_secondary_window();
}

// Callback pour changer le contenu du carré lorsqu'un bouton est cliqué
void button_clicked0(GtkWidget *widget, gpointer data) {
    const char *button_label = gtk_button_get_label(GTK_BUTTON(widget));
    char result_text[MAX_BUFFER_SIZE];
    sprintf(result_text, "%s appuyé", button_label);
    gtk_label_set_text(GTK_LABEL(result_drawing_area), result_text);
    
    send(clientSocket, "GET_DATETIME", strlen("GET_DATETIME"), 0);
    char resultBuffer[MAX_BUFFER_SIZE];
    ssize_t bytesRead = recv(clientSocket, resultBuffer, sizeof(resultBuffer), 0);
    resultBuffer[bytesRead] = '\0'; 
    sprintf(result_text, "Résultat du serveur :\n%s\n", resultBuffer);
    gtk_label_set_text(GTK_LABEL(result_drawing_area), result_text);
    
}

void button_clicked1(GtkWidget *widget, gpointer data) {
    const char *button_label = gtk_button_get_label(GTK_BUTTON(widget));
    char result_text[MAX_BUFFER_SIZE];
    sprintf(result_text, "%s appuyé", button_label);
    gtk_label_set_text(GTK_LABEL(result_drawing_area), result_text);
    send(clientSocket, "GET_FILE_LIST", strlen("GET_FILE_LIST"), 0);
    const char *directoryPath = gtk_entry_get_text(GTK_ENTRY(input_entry));
    send(clientSocket, directoryPath, strlen(directoryPath), 0);
    char resultBuffer[MAX_BUFFER_SIZE];
    ssize_t bytesRead = recv(clientSocket, resultBuffer, sizeof(resultBuffer), 0);
    resultBuffer[bytesRead] = '\0'; 
    sprintf(result_text, "Résultat du serveur :\n%s\n", resultBuffer);
    gtk_label_set_text(GTK_LABEL(result_drawing_area), result_text);
}
void button_clicked2(GtkWidget *widget, gpointer data) {
    const char *button_label = gtk_button_get_label(GTK_BUTTON(widget));
    char result_text[MAX_BUFFER_SIZE];
    sprintf(result_text, "%s appuyé", button_label);
    gtk_label_set_text(GTK_LABEL(result_drawing_area), result_text);
    send(clientSocket, "GET_FILE_CONTENT", strlen("GET_FILE_CONTENT"), 0);
        const char *filePath = gtk_entry_get_text(GTK_ENTRY(input_entry));
    send(clientSocket, filePath, strlen(filePath), 0);
    char resultBuffer[MAX_BUFFER_SIZE];
    ssize_t bytesRead = recv(clientSocket, resultBuffer, sizeof(resultBuffer), 0);
    resultBuffer[bytesRead] = '\0'; 
    sprintf(result_text, "Résultat du serveur :\n%s\n", resultBuffer);
    gtk_label_set_text(GTK_LABEL(result_drawing_area), result_text);
}
void button_clicked3(GtkWidget *widget, gpointer data) {
    const char *button_label = gtk_button_get_label(GTK_BUTTON(widget));
    char result_text[MAX_BUFFER_SIZE];
    sprintf(result_text, "%s appuyé", button_label);
    gtk_label_set_text(GTK_LABEL(result_drawing_area), result_text);
    
    
    send(clientSocket, "GET_ELAPSED_TIME", strlen("GET_ELAPSED_TIME"), 0);
    char resultBuffer[MAX_BUFFER_SIZE];
    ssize_t bytesRead = recv(clientSocket, resultBuffer, sizeof(resultBuffer), 0);
    resultBuffer[bytesRead] = '\0'; 
    sprintf(result_text, "Résultat du serveur :\n%s\n", resultBuffer);
    gtk_label_set_text(GTK_LABEL(result_drawing_area), result_text);
                   
}/*
void button_clicked4(GtkWidget *widget, gpointer data) {
    gtk_widget_destroy(GTK_WIDGET(data));
}
*/
// Callback pour redessiner le carré
gboolean draw_callback(GtkWidget *widget, cairo_t *cr, gpointer data) {
    // Dessinez le contenu du carré ici
    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0); // Couleur blanche
    cairo_paint(cr);

    // Dessinez le texte dans le carré
    cairo_set_source_rgb(cr, 1.0, 0.0, 0.0); // Couleur rouge
    cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, 14.0);

    // Coordonnées pour centrer le texte
    double x = 10.0;
    double y = 150.0;

    // Texte à afficher
    const char *text = "Le résultat sera affiché ici";

    // Dessiner le texte
    cairo_move_to(cr, x, y);
    cairo_show_text(cr, text);

    return FALSE; // False pour permettre à d'autres gestionnaires de signal de continuer à traiter l'événement
}

void create_secondary_window() {
    GtkWidget *secondary_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(secondary_window), "Secondary Window");
    gtk_container_set_border_width(GTK_CONTAINER(secondary_window), 10);
    g_signal_connect(secondary_window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Crée une boîte verticale pour contenir les widgets
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(secondary_window), vbox);

    // Crée 5 boutons
    const char *button_labels[] = {
        "la date et l'heure",
        "la liste des fichiers",
        "le contenu d'un fichier",
        "le temps écoulé"};

//,
       // ,"quitter"
   
    
    GtkWidget *button0 = gtk_button_new_with_label(button_labels[0]);
        gtk_box_pack_start(GTK_BOX(vbox), button0, TRUE, TRUE, 0);

        // Connecte le signal "clicked" pour chaque bouton
        g_signal_connect(button0, "clicked", G_CALLBACK(button_clicked0), NULL);
        
        GtkWidget *button1 = gtk_button_new_with_label(button_labels[1]);
        gtk_box_pack_start(GTK_BOX(vbox), button1, TRUE, TRUE, 0);

        // Connecte le signal "clicked" pour chaque bouton
        g_signal_connect(button1, "clicked", G_CALLBACK(button_clicked1), NULL);
        
        GtkWidget *button2 = gtk_button_new_with_label(button_labels[2]);
        gtk_box_pack_start(GTK_BOX(vbox), button2, TRUE, TRUE, 0);

        // Connecte le signal "clicked" pour chaque bouton
        g_signal_connect(button2, "clicked", G_CALLBACK(button_clicked2), NULL);

GtkWidget *button3 = gtk_button_new_with_label(button_labels[3]);
        gtk_box_pack_start(GTK_BOX(vbox), button3, TRUE, TRUE, 0);

        // Connecte le signal "clicked" pour chaque bouton
        g_signal_connect(button3, "clicked", G_CALLBACK(button_clicked3), NULL);
        
       /* GtkWidget *button4 = gtk_button_new_with_label(button_labels[4]);
        gtk_box_pack_start(GTK_BOX(vbox), button4, TRUE, TRUE, 0);

        // Connecte le signal "clicked" pour chaque bouton
        g_signal_connect(button4, "clicked", G_CALLBACK(button_clicked4), NULL);*/
        
        
        // Ajoute une zone de saisie
    input_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(input_entry), "Entrer input ici");
    gtk_box_pack_start(GTK_BOX(vbox), input_entry, TRUE, TRUE, 0);
    // Crée un label pour afficher le résultat
    GtkWidget *result_label = gtk_label_new("Le résultat sera affiché ici");
    gtk_box_pack_start(GTK_BOX(vbox), result_label, TRUE, TRUE, 0);

    // Assigne le label à la variable globale pour y accéder depuis d'autres fonctions
    result_drawing_area = result_label;

    // Connecte le signal "draw" pour redessiner le label lorsque nécessaire
    g_signal_connect(G_OBJECT(result_label), "draw", G_CALLBACK(draw_callback), NULL);

    // Affiche tous les widgets
    gtk_widget_show_all(secondary_window);
}

int main(int argc, char *argv[]) {
    // Initialise GTK
    gtk_init(&argc, &argv);

    // Obtient le nom du serveur et le port à partir des arguments de la ligne de commande
    const char *serverHostname = "127.0.0.1";
    const int serverPort = 3003;

    // Crée une socket client
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        perror("Erreur lors de la création de la socket");
        exit(EXIT_FAILURE);
    }

    // Configure l'adresse du serveur
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(serverPort);
    if (inet_pton(AF_INET, serverHostname, &serverAddr.sin_addr) <= 0) {
        perror("Adresse du serveur invalide");
        exit(EXIT_FAILURE);
    }

    // Se connecte au serveur
    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Erreur lors de la connexion au serveur");
        exit(EXIT_FAILURE);
    }

    // Crée une nouvelle fenêtre d'authentification
    GtkWidget *auth_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(auth_window), "Authentification");
    gtk_container_set_border_width(GTK_CONTAINER(auth_window), 10);
    g_signal_connect(auth_window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Crée une boîte verticale pour contenir les widgets
    GtkWidget *auth_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(auth_window), auth_vbox);

    // Crée une entrée pour le nom d'utilisateur
    GtkWidget *username_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(username_entry), "Nom d'utilisateur");
    gtk_box_pack_start(GTK_BOX(auth_vbox), username_entry, TRUE, TRUE, 0);

    // Crée une entrée pour le mot de passe
    GtkWidget *password_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(password_entry), "Mot de passe");
    gtk_entry_set_visibility(GTK_ENTRY(password_entry), FALSE);
    gtk_box_pack_start(GTK_BOX(auth_vbox), password_entry, TRUE, TRUE, 0);

    // Crée un bouton pour l'authentification
    GtkWidget *authenticate_button = gtk_button_new_with_label("Authentifier");
    g_signal_connect(authenticate_button, "clicked", G_CALLBACK(authenticate_button_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(auth_vbox), authenticate_button, TRUE, TRUE, 0);

    // Affiche tous les widgets de la fenêtre d'authentification
    gtk_widget_show_all(auth_window);

    // Lance la boucle principale GTK
    gtk_main();

    // Ferme la socket client
    close(clientSocket);

    return 0;
}
