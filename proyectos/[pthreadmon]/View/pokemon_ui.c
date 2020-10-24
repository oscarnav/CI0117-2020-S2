#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>
#include "../Model/GrandMaster.h"

GtkWidget *window;
GtkWidget *grid;

GtkWidget * info_ataque;

GtkWidget *entradas_jug1[3];
GtkWidget *entradas_jug2[3];
GtkWidget **entradas[2];


GtkWidget *labels_pokemones_jug1[3];
GtkWidget *labels_pokemones_jug2[3];
GtkWidget **labels_pokemones_jugadores[2];
//de oscar
GtkWidget *pokemones[6];
GtkWidget *pokesNombres[6];
GtkWidget *avisos_jug1;
GtkWidget *avisos_jug2;

int* idPokemones;

GtkWidget *button_start;

static void my_callback(GObject *source_object, GAsyncResult *res,
                        gpointer user_data)
{
   /* Do nothing */
}

static void start_async(GTask *task, gpointer source_object,
                        gpointer task_data, GCancellable *cancellable)
{
    int* id_pokemones = malloc((size_t)(6 * sizeof(int))); //hay que hacer free
    idPokemones = id_pokemones;

    int validosJug1 = 1;
    int validosJug2 = 1;

    for(int i =0; i<6;++i ){
      idPokemones[i] = atoi((char *)gtk_entry_get_text(GTK_ENTRY(pokemones[i])));
      
        if(idPokemones[i]< 0 || idPokemones[i]>49){
            if(i < 3){
                validosJug1 = 0;
            }
            else{
                validosJug2 = 0;
            }
        }
    }

    int revisarPoke = 0;
    int por_analizar = 2;
    while(revisarPoke < 6){
        for(int i = 0; i < por_analizar; ++i){
            if(idPokemones[revisarPoke] == idPokemones[revisarPoke+i+1]){
                if(revisarPoke < 3){
                    validosJug1 = 0;
                }
                else{
                    validosJug2 = 0;    
                }
            }
       }
       if(revisarPoke == 2){
           por_analizar = 2;
       }
       else{
           --por_analizar;
       }
        ++revisarPoke;
    }

    if(validosJug1 && validosJug2){
       
       initialize_data();
       for(int i =0; i<6;++i ){
         gtk_widget_set_sensitive(pokemones[i],FALSE);
         gtk_label_set_text(GTK_LABEL(pokesNombres[i]), get_pokemon_species_name(idPokemones[i]));
       }
       //gtk_widget_hide(button_start);
       gtk_label_set_text(GTK_LABEL(avisos_jug1), "Avisos: \n" );
       gtk_label_set_text(GTK_LABEL(avisos_jug2), "Avisos: \n" );
       iniciarBatalla(idPokemones);
       //gtk_widget_show(button_start);
       for(int i =0; i<6;++i ){
         gtk_widget_set_sensitive(pokemones[i],TRUE);
       }
    }
    else{
        if(!validosJug1){
            gtk_label_set_text(GTK_LABEL(avisos_jug1), "Avisos: \n Pokemones invalidos" );    
        }
        else{
            gtk_label_set_text(GTK_LABEL(avisos_jug1), "Avisos: \n" ); 
        }
        if(!validosJug2){
            gtk_label_set_text(GTK_LABEL(avisos_jug2), "Avisos: \n Pokemones invalidos" );
        }
        else{
            gtk_label_set_text(GTK_LABEL(avisos_jug2), "Avisos: \n" );
        }
        printf("Pokemones no validos\n");
    }
    
}

static void start_clicked()
{
    GCancellable *cancellable = g_cancellable_new();
    GTask *task = g_task_new(g_object_new(G_TYPE_OBJECT, NULL), cancellable, my_callback, NULL);
    g_task_run_in_thread(task, start_async);
    g_object_unref(task);
}





static gboolean draw_arena(GtkWidget *widget, GdkEventExpose *event, gpointer data){
    
    float pokeActivoHP = 0;
    int energiaRequerida = 0;
    char buffer[60];
    int pokeDelJugActivo1 = 0;
    int pokeDelJugActivo2 = 0;
    int pokesDeJugadoresActivos[2];
    if(shared_data_global){

        pokeDelJugActivo1 = shared_data_global->pokeDelJugActivo[0];
        pokeDelJugActivo2 = shared_data_global->pokeDelJugActivo[1];
        pokesDeJugadoresActivos[0] = pokeDelJugActivo1;
        pokesDeJugadoresActivos[1] = pokeDelJugActivo2;

        /*Ataque actual para pokemon activo del jugador 1*/
        for(int i = 0; i < 2; ++i){
            if(shared_data_global->ataqueActual[i] == 1){
                snprintf(buffer, sizeof(buffer), "Ataque:\n %s \n ha usado \n %s", shared_data_global->pokemones[pokesDeJugadoresActivos[i]]->species_name, shared_data_global->pokemones[pokesDeJugadoresActivos[i]]->charged_move_data->moveName);
                gtk_label_set_text(GTK_LABEL(info_ataque), buffer);
            }
            else{
                if(shared_data_global->ataqueActual[i] == 2){
                    snprintf(buffer, sizeof(buffer), "Ataque:\n %s \n ha usado \n %s", shared_data_global->pokemones[pokesDeJugadoresActivos[i]]->species_name, shared_data_global->pokemones[pokesDeJugadoresActivos[i]]->fast_move_data->moveName);
                    gtk_label_set_text(GTK_LABEL(info_ataque), buffer);
                }
            }
        }
        
        /*vida del pokemon activo del jugador 1*/
        pokeActivoHP = *shared_data_global->HP[pokeDelJugActivo1];
        snprintf(buffer, sizeof(buffer), "%i/1500", (int)pokeActivoHP);
        gtk_label_set_text(GTK_LABEL(labels_pokemones_jugadores[0][0]), buffer);

        /*vida del pokemon activo del jugador 2*/
        pokeActivoHP = *shared_data_global->HP[pokeDelJugActivo2];
        snprintf(buffer, sizeof(buffer), "%i/1500", (int)pokeActivoHP);
        gtk_label_set_text(GTK_LABEL(labels_pokemones_jugadores[1][0]), buffer);

        /*Nombre del pokemon activo del jugador 1*/
        gtk_label_set_text(GTK_LABEL(labels_pokemones_jugadores[0][1]), shared_data_global->pokemones[pokeDelJugActivo1]->species_name);
        
        /*Nombre del pokemon activo del jugador 2*/
        gtk_label_set_text(GTK_LABEL(labels_pokemones_jugadores[1][1]), shared_data_global->pokemones[pokeDelJugActivo2]->species_name);
    
        /*Energia acumulada del pokemon activo del jugador 1*/
        energiaRequerida = shared_data_global->pokemones[pokeDelJugActivo1]->charged_move_data->energy;
        snprintf(buffer, sizeof(buffer), "%i/%i", shared_data_global->energias_acumuladas[pokeDelJugActivo1], energiaRequerida);
        gtk_label_set_text(GTK_LABEL(labels_pokemones_jugadores[0][2]), buffer);

        /*Energia acumulada del pokemon activo del jugador 2*/
        energiaRequerida = shared_data_global->pokemones[pokeDelJugActivo2]->charged_move_data->energy;
        snprintf(buffer, sizeof(buffer), "%i/%i", shared_data_global->energias_acumuladas[pokeDelJugActivo2], energiaRequerida);
        gtk_label_set_text(GTK_LABEL(labels_pokemones_jugadores[1][2]), buffer);

        int pokes_debilitados = 0;
        int poke_vivo = 0;
        for(int i = 0; i < 6; ++i){
            if(*shared_data_global->HP[i] <= 0){
                snprintf(buffer, sizeof(buffer), "%s: %is", get_pokemon_species_name(idPokemones[i]), (int)shared_data_global->duraciones[i]);
                gtk_label_set_text(GTK_LABEL(pokesNombres[i]), buffer);
                ++pokes_debilitados;
            }
            else{
                poke_vivo = i;
            }
        }
        if(pokes_debilitados >= 5 ){
            snprintf(buffer, sizeof(buffer), "%s: %is", get_pokemon_species_name(idPokemones[poke_vivo]), (int)shared_data_global->duraciones[poke_vivo]);
            gtk_label_set_text(GTK_LABEL(pokesNombres[poke_vivo]), buffer);
            if(poke_vivo < 3){
                gtk_label_set_text(GTK_LABEL(avisos_jug1), "Gana el retador 1");
            }
            else{
                gtk_label_set_text(GTK_LABEL(avisos_jug1), "Gana el retador 2");
            }
            snprintf(buffer, sizeof(buffer), "Tiempo de batalla total: \n   %is", (int)shared_data_global->duraciones[6]);
            gtk_label_set_text(GTK_LABEL(avisos_jug2), buffer);
            pthread_cond_signal(&shared_data_global->condVariable[6]);
        }
        else{
            gtk_label_set_text(GTK_LABEL(avisos_jug1), "");
            gtk_label_set_text(GTK_LABEL(avisos_jug2), "");
        }
    }
    return TRUE;
}


static void
activate(GtkApplication *app,
         gpointer user_data)
{

    /* create a new window, and set its title */
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Poke Batalla");
    gtk_window_set_default_size(GTK_WINDOW(window), 200, 200);
    gtk_container_set_border_width(GTK_CONTAINER(window), 40);

    /* construct the container and sets the rows / columns to be of the same size (homogeneeous) */
    grid = gtk_grid_new();
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
    //gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);

    /* Put the grid in the window */
    gtk_container_add(GTK_CONTAINER(window), grid);

    /* create the button and set the event 'clicked' */
    button_start = gtk_button_new_with_label("START");
    g_signal_connect(button_start, "clicked", G_CALLBACK(start_clicked), NULL);
    
    
    for(int i =0; i<6;++i ){
        pokemones[i] = gtk_entry_new();
        pokesNombres[i] = gtk_label_new(".........");
    }
        avisos_jug1 = gtk_label_new("Avisos: \n");
        avisos_jug2 = gtk_label_new("Avisos: \n");

    /* place the button in the grid cell (2, 9), width=2 heigth=1 */
    gtk_grid_attach(GTK_GRID(grid), button_start, 4, 9, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), avisos_jug1, 3, 9, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), avisos_jug2, 5, 9, 1, 1);
    /*Aqui iria la info del ataque*/

    info_ataque = gtk_label_new("ATAQUES");
    gtk_grid_attach(GTK_GRID(grid), info_ataque, 3, 1, 2, 5);
    /*Aqui irian las entradas*/
    int h_de_entradas = 1;
    entradas[0] = entradas_jug1;
    entradas[1] = entradas_jug2;  
    for(int j = 0; j < 2; ++j){
        h_de_entradas = 1;
        for (int i = 0; i < 3; ++i) {
            entradas[j][i] = gtk_label_new("::");
            //gtk_grid_attach(GTK_GRID(grid), entradas[j][i], j*6, h_de_entradas, 1, 2);
            h_de_entradas += 2;
        }
    }

    /*crear la info de los pokemones en pelea*/

    labels_pokemones_jugadores[0] = labels_pokemones_jug1;
    labels_pokemones_jugadores[1] = labels_pokemones_jug2;
    int y = 1;
    int sumarleAY = 0;
    int alto = 0;
    for(int j = 0; j < 2; ++j){
        y = 1;
        for (int i = 0; i < 3; ++i) {
            if(i == 0){
                labels_pokemones_jugadores[j][i] = gtk_label_new("HP");
                sumarleAY = 4;
                alto = 1;
            }
            else{
                if(i == 1){
                    labels_pokemones_jugadores[j][i] = gtk_label_new("-");
                    sumarleAY = 13;
                    alto = 3;
                }
                else{
                    labels_pokemones_jugadores[j][i] = gtk_label_new("Energy: ");
                    alto = 1;
                }
            }
            gtk_grid_attach(GTK_GRID(grid), labels_pokemones_jugadores[j][i], j*3 + 2, y, 1, alto);
            y += alto;
        }
    }
    



    //DE oscar
    
     
    //Para ids de pokemones de  jugador 1
    for(int i =0; i<3;++i ){
    gtk_grid_attach(GTK_GRID(grid), pokemones[i], 0, i*3, 1, 1);
    }
    //Para ids de pokemones de  jugador 2
    for(int i =3; i<6;++i ){
    gtk_grid_attach(GTK_GRID(grid), pokemones[i], 8, (i-3)*3, 1, 1);
    }
            
            
    //Para nombres de pokemones de  jugador 1
    for(int i =0; i<3;++i ){
    //gtk_label_set_text(GTK_LABEL(pokesNombres[i]), "_____");
    gtk_grid_attach(GTK_GRID(grid), pokesNombres[i], 1, i*3, 1, 1);
    }
            
            
    //Para nombres de pokemones de  jugador 2
    for(int i =3; i<6;++i ){
    gtk_grid_attach(GTK_GRID(grid), pokesNombres[i], 7, (i-3)*3, 1, 1);
    }

    


    g_timeout_add(33, (GSourceFunc)draw_arena, (gpointer)window);
    
    
    /* Show all the widgets in the window */
    gtk_widget_show_all(window);
}

int main(int argc, char* argv[]) {

    GtkApplication *app;
    int status;

    app = gtk_application_new("example.gtk.running_track", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;

}
