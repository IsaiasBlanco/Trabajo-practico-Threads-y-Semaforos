# **SOR-Semáforos-1S-2021**
# Alumno: Blanco, Isaias Ezequiel
# Docentes: Mariano Vargas, Noelia Sosa, Ignacio Tula
## *Subway Argento*



**El siguiente proyecto consiste en 4 equipos que compiten por terminar primero un sándwich. Cada equipo comienza al mismo tiempo pero con la particularidad de que la cocina no cuenta con el equipamiento individual para cada equipo, sino que algunos de ellos son compartidos**

## ***Desarrollo***
_Para iniciar a realizar la competencia, lo primero que me propuse fue a realizar un pseudocódigo, gracias a esto fue más claro al momento de implementarlo en **lenguaje C**. Para poder realizar el pseudocódigo, el primer paso fue entender que acciones existen y que acción habilita a que acción._

### Acciones
- Cortar ajo y perejil
  - Mezclar el ajo y perejil con huevo
    - Poner sal a la mezcla
      - Habilita el armado del sándwich
- Agregar carne
  - Empanar carne
    - Cocinar carne
      - Habilita el armado del sándwich
- Hornear panes
  - Habilita el armado del sándwich
- Cortar lechuga fresca, tomate, cebolla morada y pepino
  - Habilita el armado del sándwich

*Dada la siguiente lista se pudo resolver el problema de paralelismo en las tareas y las tareas que **habilitan** el armado del sándwich*


## ***Pseudocódigo***

### Equipamiento compartido _(declarado en semaforos mutex)_
| Nombre Semáforo Mutex  | Valor inicial  |
| :------------ | :------------
| m_salero | 0 |
| m_sarten | 0 |
| m_ganador | 0 |

*Como se observa, decidí utilizar un semáforo llamado _"ganador"_, el cual es inicializado en 0 ya que solo puede haber un solo ganador. A su vez utilizo un mutex para el salero y otro para el sarten ya que solo pueden utilizarlo un solo equipo a la vez.*

### Equipamiento de los equipos _(declarado en semáforos contadores)_
| Nombre Semáforo Contador | Valor inicial  |
| :------------ | :------------
| sem_mezclar | 0 |
| sem_salero | 0 |
| sem_empanar | 0 |
| sem_sarten | 0 |
| sem_horno | 2 |
| sem_sandwich0 | 0 |
| sem_sandwich1 | 0 |
| sem_sandwich2 | 0 |
| sem_sandwich3 | 0 |

*Asimismo se toman los siguientes procesos para la simulación de las actividades y/o acciones a realizar:*

| Procesos | Acción  |
| :------------ | :------------ |
|  P1 | Cortar ajo y perejil |
|  P2 | Mezclar ajo y perejil con huevo |
|  P3 | Condimentar ajo, perejil y huevo |
|  P4 | Agregar carne  |
|  P5 | Empanar carne |
|  P6 | Cocinar carne |
|  P7 | Hornear el pan |
|  P8 | Picar lechuga, tomate, cebolla y pepino |
|  P9 | Armar el sándwich |

### Prueba de escritorio

| Nombre Semáforo  | Valor Inicial  | ||||||
| :------------ | :------------ | :------------ | :------------ | :------------ | :------------ | :------------ |  :------------ | 
| sem_mezclar   | 0 | 4 | 0 | 0 | 0 | 0 | 0
| sem_salero    | 0 | 0 | 1 | 0 | 1 | 0 | 1
| sem_empanar   | 0 | 4 | 0 | 0 | 0 | 0 | 0
| sem_sarten    | 0 | 0 | 1 | 0 | 1 | 0 | 1
| sem_horno     | 2 | 2 | 0 | 0 | 2 | 0 | 0
| sem_sandwich0 | 0 | 0 | 0 | 1 | 1 | 1 | 1
| sem_sandwich1 | 0 | 0 | 0 | 1 | 1 | 1 | 1
| sem_sandwich2 | 0 | 0 | 1 | 1 | 1 | 1 | 1
| sem_sandwich3 | 0 | 0 | 1 | 1 | 1 | 1 | 1

Para este apartado se tiene en cuenta que:

- P( ) --> wait
- V( ) --> signal

| P1  | P2  | P3  | P4  | P5  | P6 |  P7 | P8 | P9
| ------------ | ------------ | ------------ | ------------ | ------------ | ------------ | ------------ |------------ | ------------ |
||||||||| P(sem_sandwich0) |
||||||||| P(sem_sandwich1) | 
||||||||| P(sem_sandiwch2) |
|| P(sem_mezclar) | P(sem_salero) || P(sem_sarten) | P(sem_cocinar) ||| P(sem_sandwich3)
| Cortar | Mezclar | Salar | Agregar | Empanar | Cocinar | Hornear | Picar | Armar |
| V(sem_mezclar) | V(sem_salero) |V(sem_sandwich0) | V(sem_sarten) | V(sem_cocinar) | V(sem_sandwich1) | V(sem_sandwich2) | V(sem_sandiwch3)|



## Lectura del archivo
Una vez realizado el pseudocódigo, el siguiente paso fue el de leer el archivo llamado ***"receta.txt"*** el cuál contenía lo siguiente
```
cortar ajo y perejil
agregar carne
hornear pan
mezclar ajo y perejil con huevo
empanar carne
salar ajo, perejil y huevo
cocinar carne
tajar lechuga, tomate, cebolla y pepino
armar sándwich
```
Esto fue posible a la siguiente función.

```
char linea[50];
        FILE *leer_receta;
        leer_receta = fopen("receta.txt", "r");
        for (int i = 0; i < 9; i++)                  // Leo cada linea del archivo "receta"
        {
                fgets (linea, 50, leer_receta);      // Lee el buffer hasta que halla una "\n"
                escribir_en_archivo(linea,0);        // Escribe la receta en el archivo "subwayArgentoGanador.txt"
        }
```

_Esta función se encarga de leer el archivo llamado ***receta.txt*** y lo almacena en una variable llamada ***leer_receta*** la cual luego nos permitirá escribir en el archivo lo cual me lleva al siguiente punto._



## Escritura del archivo
_Una vez realizada la lectura del archivo, el siguiente paso fue el de escribir cada acción de cada equipo en un archivo. Para poder llevar a cabo este proceso modifique la función ***ejecutarReceta***, la cual recibe como parámetro el numero de equipo que se va a querer escribir en el archivo y la acción realizada por el mismo, esta información la recibe para poder determinar que número de equipo se está ejecutando. De esta manera en el archivo de salida se obtendrá la siguiente secuencia:_
- Equipo 1: Cortando Ajo y Perejil
- Equipo 2: Cortando Ajo y Perejil
- Equipo 3: Cortando Ajo y Perejil
- Equipo 4: Cortando Ajo y Perejil
- ***etc.***


### Problema encontrado
_El problema que encontré fue al realizar la ejecución de cada acción a medida que leía el archivo llamado ***receta.txt***, la solución planteada a este problema fue la siguiente:_ 



```
void escribir_en_archivo(char msg[], int equipo)	// Función que escribe las acciones de cada equipo
{
        FILE* competencia;
        competencia = fopen("subwayArgentoGanador.txt", "a");
        fprintf (competencia,msg,equipo);
        fclose(competencia);
}
```


```
void *ejecutarReceta(void* numero)
{
        int equipo = *(int*)numero;  			// Casteo de puntero a un entero
        char linea[50];
        FILE *receta;
        receta = fopen("receta.txt", "r");
	    for (int i=0; i<9;  i++) {
                fgets (linea, 50,  receta);		// Leo cada linea de la receta
                if (strstr(linea, "cortar") != NULL)	// Si la linea contiene esa palabra
                {
		  	cortar_ajo_y_perjil(equipo);	// Ejecuto la función
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
```


_La siguiente función se encarga de leer cada linea del archivo y si la linea contiene la palabra ***clave*** o si hay un salto de linea se ejecutara la función correspondiente. Por ejemplo: al leer "cortar", se ejecutara la siguiente función._
```
void cortar_ajo_y_perjil(int equipo)
{
        escribir_en_archivo("Equipo %d: cortando ajo y perejil\n",equipo);
        usleep(100000);
        sem_post(&sem_mezclar);
}
```


## Sincronización
**Una vez que son ejecutadas las funciones hornear_pan(equipo), salar(equipo), cocinar_carne(equipo) y cortar_verduras(equipo) estas habilitaran a cuatro semáforos distintos, los cuales están sincronizados con la función llamada armar_sandwich(equipo). La acción para armar el sándwich depende de los cuatro semáforos, una vez que estos son habilitados, se puede armar el sándwich y a su vez informar qué equipo ganó.**

## Conclusión
**Si bien fue un trabajo difícil, ya que por mi parte tengo poca experiencia con el lenguaje C. Este trabajo me ayudo a entender el funcionamiento de hilos y semáforos más claramente, sin embargo, la mayor dificultad de este trabajo fue la de el manejo de archivos, ya que presento varias dificultades mencionadas anteriormente.**
