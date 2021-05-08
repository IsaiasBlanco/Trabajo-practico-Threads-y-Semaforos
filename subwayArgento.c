#include <stdio.h>	// libreria estandar
#include <stdlib.h>	// para usar exit y funciones de la libreria estandar
#include <string.h>
#include <pthread.h>	// para usar threads
#include <semaphore.h>	// para usar semaforos
#include <unistd.h>

// Declaro los semaforos
sem_t sem_mezclar;
sem_t sem_salero;
sem_t sem_empanar;
sem_t sem_sarten;
sem_t sem_horno;
sem_t sem_sandwich0;
sem_t sem_sandwich1;
sem_t sem_sandwich2;
sem_t sem_sandwich3;

// Declaro los mutex
static pthread_mutex_t m_salero;
static pthread_mutex_t m_sarten;
static pthread_mutex_t ganador;

// Identificador de cada equipo
int equipo_ganador = 0;

// Funcion que se encarga de escribir en un archivo
void escribir_en_archivo(char msg[], int equipo);

// Funciones de cada accion
void cortar_ajo_y_perjil(int equipo)
{
        escribir_en_archivo("Equipo %d: cortando ajo y perejil\n",equipo);
        usleep(100000);
        sem_post(&sem_mezclar);
}


void mezclar_con_huevo(int equipo)
{
        sem_wait(&sem_mezclar);
        escribir_en_archivo("Equipo %d: terminó de cortar ajo y perejil\n\n", equipo);
        escribir_en_archivo("Equipo %d: mezclando ajo y perejil con huevo!\n", equipo);
        usleep(100000);
	sem_post(&sem_salero);
}


void salar(int equipo)
{
        sem_wait(&sem_salero);
        escribir_en_archivo("Equipo %d: terminó de mezclar! ahora a salar!\n\n", equipo);
        pthread_mutex_lock(&m_salero);                   // El primer equipo en entrar a la seccion critica
        escribir_en_archivo("Equipo %d: condimentando el ajo, perejil y el huevo!\n", equipo);
        usleep(200000);
        escribir_en_archivo("Equipo %d: terminó de condimentar el ajo, perejil y el huevo!\n\n", equipo);
        pthread_mutex_unlock(&m_salero);                   // El primer equipo en entrar a la seccion critica
sem_post(&sem_sandwich0);
}


void hornear_pan(int equipo)
{
        sem_wait(&sem_horno);
        escribir_en_archivo("Equipo %d: horneando pan\n", equipo);
        usleep(400000);
        escribir_en_archivo("Equipo %d: pan listo!\n\n", equipo);
	sem_post(&sem_horno);
sem_post(&sem_sandwich1);
}


void agregar_carne(int equipo)
{
        escribir_en_archivo("Equipo %d: agregó la carne\n", equipo);
        usleep(100000);
        sem_post(&sem_empanar);
}


void empanar_carne(int equipo)
{
        sem_wait(&sem_empanar);
        usleep(100000);
        escribir_en_archivo("Equipo %d: termino de empanar la carne!\n\n", equipo);
	sem_post(&sem_sarten);
}


void cocinar_carne(int equipo)
{
        sem_wait(&sem_sarten);
	    pthread_mutex_lock(&m_sarten);
        	escribir_en_archivo("Equipo %d: cocinando carne!\n", equipo);
        	usleep(200000);
        	escribir_en_archivo("Equipo %d: carne cocida!\n\n", equipo);
	    pthread_mutex_unlock(&m_sarten);
sem_post(&sem_sandwich2);
}


void cortar_verduras(int equipo)
{
        escribir_en_archivo("Equipo %d: está picando lechuga, tomate, cebolla y pepinos!\n", equipo);
        usleep(400000);
        escribir_en_archivo("Equipo %d: terminó de picar lechuga, tomate, cebolla y pepinos\n\n", equipo);
sem_post(&sem_sandwich3);
}


void armar_sandwich(int equipo)
{
        sem_wait(&sem_sandwich0);
	sem_wait(&sem_sandwich1);
        sem_wait(&sem_sandwich2);
	sem_wait(&sem_sandwich3);
        escribir_en_archivo("\nEquipo %d: Preparando el sandwich!\n", equipo);
        pthread_mutex_lock(&ganador); 			// El primer equipo en entrar a la seccion critica
        if (equipo_ganador == 0)
        {
                equipo_ganador = equipo;		// Asigno al primer equipo en entrar a la saccion critica
                escribir_en_archivo("\n---------Equipo %d: Primero en entregar el sandwich!---------\n\n",equipo);		// Escribo en el archivo
                printf("El equipo %d ya presentó el sandwich!\n\n", equipo);					// Muestro por pantalla al finalizar la ejecución
        }
        pthread_mutex_unlock(&ganador); 		// El primer equipo en salir de la saccion critica
}


// Función que escribe las acciones de cada equipo en el archivo llamado 'subwayArgentoGanador.txt'
void escribir_en_archivo(char msg[], int equipo)
{
        FILE* competencia;
        competencia = fopen("subwayArgentoGanador.txt", "a");
        fprintf (competencia,msg,equipo);
        fclose(competencia);
}


void *ejecutarReceta(void* numero)
{
        int equipo = *(int*)numero;  			    // Casteo de puntero a un entero
        char linea[50];
        FILE *receta;
        receta = fopen("receta.txt", "r");
	      for (int i=0; i<9;  i++) {
                fgets (linea, 50,  receta);		    // Leo cada linea de la receta
                if (strstr(linea, "cortar") != NULL)	    // Si la linea contiene esa palabra
                {
			cortar_ajo_y_perjil(equipo);	    // Ejecuto la función
                }
                if (strstr(linea, "agregar") != NULL)
                {
                        agregar_carne(equipo);
                }
                if (strstr(linea, "hornear") != NULL)
                {
                        hornear_pan(equipo);
                }
                if (strstr(linea,"mezclar") != NULL)
                {
                        mezclar_con_huevo(equipo);
                }
                if (strstr(linea, "empanar") != NULL)
                {
                        empanar_carne(equipo);
                }
                if (strstr(linea, "salar") != NULL)
                {
                        salar(equipo);
                        printf("El equipo %d esta salando.\n\n", equipo);
		}
                if (strstr(linea, "cocinar") != NULL)
                {
                        printf("El equipo %d tiene su carne lista.\n\n", equipo);
                        cocinar_carne(equipo);
                }
                if (strstr(linea, "picar") != NULL)
                {
                        cortar_verduras(equipo);
                        printf("El equipo %d esta cortando verduras.\n\n", equipo);
                }
                if (strstr(linea, "armar") != NULL)
                {
                        printf("El equipo %d está armando el sandwich\n",equipo);
                        armar_sandwich(equipo);
                }
	      }
        pthread_exit(NULL);
}


int main()
{
        FILE *competencia;
        char nombre_archivo[] = "subwayArgentoGanador.txt";
        competencia = fopen(nombre_archivo, "w");
        int elimino = remove (nombre_archivo);			//Elimino el archivo cada vez que es ejecutado

        char linea[50];
        FILE *leer_receta;
        leer_receta = fopen("receta.txt", "r");
        for (int i = 0; i < 9; i++)			// Escribe la receta en el archivo "subwayArgentoGanador.txt"
        {
                fgets (linea, 50, leer_receta);
                escribir_en_archivo(linea,0);
        }

  
	// Inicializo los semaforos
        sem_init(&sem_mezclar, 0, 0);
        sem_init(&sem_salero, 0, 0);
	sem_init(&sem_empanar, 0, 0);
        sem_init(&sem_sarten, 0, 0);
        sem_init(&sem_horno, 0, 2);
        sem_init(&sem_sandwich0, 0, 0);
        sem_init(&sem_sandwich1, 0, 0);
        sem_init(&sem_sandwich2, 0, 0);
        sem_init(&sem_sandwich3, 0, 0);

  
	// Inicializo el mutex
        pthread_mutex_init(&m_salero, NULL);
        pthread_mutex_init(&m_sarten, NULL);
        pthread_mutex_init(&ganador, NULL);
  
  
        // Creo los nombres de los equipos
        int equipoNombre1 = 1;
        int equipoNombre2 = 2;
        int equipoNombre3 = 3;
        int equipoNombre4 = 4;


	// Creo las variables de los hilos de los equipos
	pthread_t equipo1;
	pthread_t equipo2;
	pthread_t equipo3;
	pthread_t equipo4;


	// Inicializo los hilos de los equipos
        pthread_create(&equipo1,			//identificador unico
			        NULL,			//atributos del thread
			            ejecutarReceta,	//funcion a ejecutar
			          &equipoNombre1);	//parametros de la funcion a ejecutar, pasado por referencia

        pthread_create(&equipo2,
			        NULL,
			            ejecutarReceta,
			          &equipoNombre2);

        pthread_create(&equipo3,
			        NULL,
			            ejecutarReceta,
			          &equipoNombre3);

        pthread_create(&equipo4,
			        NULL,
			            ejecutarReceta,
			          &equipoNombre4);


	// Join de los hilos
        pthread_join(equipo1, NULL);
	pthread_join(equipo2, NULL);
	pthread_join(equipo3, NULL);
        pthread_join(equipo4, NULL);


	// Destruyo los semaforos
        sem_destroy(&sem_mezclar);
        sem_destroy(&sem_salero);
	sem_destroy(&sem_empanar);
	sem_destroy(&sem_sarten);
	sem_destroy(&sem_horno);
	sem_destroy(&sem_sandwich0);
        sem_destroy(&sem_sandwich1);
        sem_destroy(&sem_sandwich2);
        sem_destroy(&sem_sandwich3);


	// Destruyo los mutex
        pthread_mutex_destroy(&m_salero);
        pthread_mutex_destroy(&m_sarten);
        pthread_mutex_destroy(&ganador);


        escribir_en_archivo("\nEL EQUIPO %d FUE EL PRIMERO EN TERMINAR EL SANDWICH.\n", equipo_ganador);
        printf("\n\nEL EQUIPO %d FUE EL PRIMERO EN TERMINAR EL SANDWICH\n",equipo_ganador);


        char cadena[1];
        printf("\nMostrar el proceso de la competencia?\nResponder s o n\n");
        scanf("%s", cadena);

        if (strcmp(cadena,"s") == 0) // Si la respuesta fue s, muestro el archivo
        {
                char linea[50];
                FILE *arch;
                printf("\nEl archivo subwayArgentoGanador.txt contiene lo siguiente: \n");
	        printf("\nReceta: \n");
                arch = fopen("subwayArgentoGanador.txt", "r");
                while (fgets (linea, 50, arch) != NULL)
                {
                        printf("%s", linea);
                }
        }
        else // Si la respuesta es n, termina el programa
        {
                printf("La competencia se encuentra en el archivo llamado 'subwayArgentoGanador.txt', si lo quiere visualizar use el comando: cat subwayArgentoGanador.txt\n");
        }
        pthread_exit(NULL);
}
