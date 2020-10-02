#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
	pthread_mutex_t mutex;
	float delta_x;
	float area_total;
	float a;
} shared_data_t;

typedef struct {
    size_t thread_num;//podriamos prescindir
	int rectangulosAsignados;
	int posRectangulo;
	float thread_h;//tal vez lo quitamos y poner imagen
	float thread_preimagen;
    shared_data_t* shared_data;
} thread_data_t;

/* Para 4 threads, n = 6, a = 15, b = 24;
Primer thread:
	0*1,5 + 15 = 15
	1*1,5 + 15 = 16,5
Segundo thread:
	2*1,5 + 15 = 18
	3*1,5 + 15 = 19,5
Tercer thread:
	4*1,5 + 15 = 21
Cuarto thread:
	5*1,5 + 15 = 22,5
*/

void* calcularArea(void* args) {
        
    thread_data_t* data = args;
    size_t thread_num = data->thread_num;//podriamos prescindir
    shared_data_t* shared_data = data->shared_data;
    float delta_x = shared_data->delta_x;
    float area = 0;
	float preimagen = 0;
	while(data->rectangulosAsignados){
		preimagen =  data->posRectangulo * delta_x + shared_data->a;
		float imagen_x = (preimagen*preimagen)+1; // funcion de calculo de imagen iria antes del lock
		++data->posRectangulo;
		--data->rectangulosAsignados;
		area += imagen_x*delta_x;         //calcular area aqui
	}
	
    pthread_mutex_lock(&shared_data->mutex);
    shared_data->area_total+=area;
    pthread_mutex_unlock(&shared_data->mutex);
    return NULL;
}


int main(int argc, char* arg[]) {



    float punto_a = 0;
	float punto_b = 0;
	size_t n_rectangulos = 0;
    size_t thread_count = 0;
	
	
    if (argc >= 5) {
        punto_a = (float)strtoul(arg[1], NULL, 10);
		punto_b = (float)strtoul(arg[2], NULL, 10);
		n_rectangulos = (size_t)strtoul(arg[3], NULL, 10);
		thread_count = (size_t)strtoul(arg[4], NULL, 10);
		
    } else {
        fprintf(stderr, "Error, invalid number of parameters\n");
        return 1;
    }
	/*crear arreglos*/
    pthread_t* threads = malloc((size_t)(thread_count * sizeof(pthread_t)));
    shared_data_t* shared_data = (shared_data_t*)calloc(1, sizeof(shared_data_t));
    thread_data_t* thread_data = malloc((size_t)(thread_count * sizeof(thread_data_t)));
	
	/*Inicializar mutex*/
    pthread_mutex_init(&shared_data->mutex, /*attr*/ NULL); 
    
	/*Asignacion de shared data*/
    shared_data->delta_x = (punto_b-punto_a)/n_rectangulos;
    shared_data->area_total = 0;  //se inicializa el area en 0
	shared_data->a = punto_a;
    
	/*Averiguar cuantos rectangulos le toca resolver a cada thread*/
	size_t residuo = 0;
	size_t cociente = 0;
	if(n_rectangulos < thread_count){
		thread_count = n_rectangulos;
		residuo = thread_count;
	}
	else{
		cociente = n_rectangulos/thread_count;
		residuo = n_rectangulos%thread_count;
	}
	
	/*Asignacion de rectangulos para cada thread*/
	int numRectangulo = 0;
	for(size_t i; i < thread_count; ++i){
		thread_data[i].posRectangulo = numRectangulo;
		thread_data[i].rectangulosAsignados = cociente;
		if(residuo > 0){
			++thread_data[i].rectangulosAsignados;
			--residuo;
		}
		numRectangulo += thread_data[i].rectangulosAsignados;
	}
	
	/*Crear los threads*/
    for (size_t i = 0; i < thread_count; ++i){
        thread_data[i].thread_num = i;
        thread_data[i].shared_data = shared_data;
        pthread_create(&threads[i], NULL, calcularArea, (void*)&thread_data[i]);
    }

    for (size_t i = 0; i < thread_count; ++i){
        pthread_join(threads[i], NULL);
    }
	printf("El area total es %f\n", shared_data->area_total);

	pthread_mutex_destroy(&shared_data->mutex);
    free(threads);
    free(shared_data);
    free(thread_data);

    return 0;
}
