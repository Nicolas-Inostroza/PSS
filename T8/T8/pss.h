//==========================================
// Funciones para el TDA HashMap
//==========================================

typedef struct map Map;
typedef unsigned (*HashFun)(void *ptr);
typedef int (*EqualsFun)(void *ptrX, void *ptrY);
typedef struct map_iterator MapIterator;

// makeHashMap construye un mapa asociativo basado en una tabla de hash
// de tamano size.  Se debe prover la funcion que calcula el hash de las
// llaves.  Debe tener este encabezado por ejemplo:
//   unsigned int hash(void *ptr);
// Tambien se debe proveer la funcion que compara las llaves con este
// encabezado:
//   int equals(void *ptrx, void *ptry);
// equals debe entregar verdadero si ptrx es igual a ptry

Map *makeHashMap(int size, HashFun hash, EqualsFun equals);

void destroyHashMap(Map *map);     // Destructor
int contains(Map *map, void *key); // Verdadero si key esta en map, falso si no.
void *query(Map *map, void *key);  // Entrega el valor asociado a key.
int define(Map *map,               // Establece que el valor de key es
           void *key, void *val);  // val.
void *del(Map *map, void *key);    // Elimina la llave key, no libera la memoria
                                   // memoria ocupada por key.
MapIterator *getMapIterator(Map *map);  // Construye un iterador para map.
void destroyMapIterator(MapIterator *iter);
int mapNext(MapIterator *iter,          // Entrega en *pkey y *pval la
            void **pkey, void **pval);  // siguiente llave y su valor asociado.
int mapHasNext(MapIterator *iter);      // Verdadero si hay llaves por visitar.
void resetMapIterator(MapIterator *iter); // Para volver a iterar con iter.

// Funcion de hash y de igualdad para mapas de punteros

unsigned hash_ptr(void *ptr);
int pointer_equals(void *ptr1, void* ptr2);

// Funcion de hash y de igualdad para mapas de strings

unsigned hash_string(void *str);
int equals_strings(void *ptrX, void *ptrY);

// Funcion de hash y de igualdad para mapas con llaves enteras (tipo intptr_t).
// Para poder agregar asociaciones tiene que usar casts.  Por ejemplo:
//    Map *map= makeHashMap(100, hash_int, equals_int);
//    intptr_t i = ...
//    define(map, (void*) i, "i");
//    char *s= query(map, (void*)i);  // s es "i"

unsigned hash_int(void *ptr);
int equals_int(void *ptrI, void *ptrJ);

//==========================================
// Funciones para el TDA Queue
//==========================================

typedef struct queue Queue;

Queue *makeQueue();            // Construye una cola FIFO
void destroyQueue(Queue *q);   // Destructor
void put(Queue *q, void *ptr); // Agrega ptr al final de q
void *get(Queue *q);           // Extrae y retorna el primer elemento de q
void *peek(Queue *q);          // Entrega el primer elemento de q sin extraerlo
int emptyQueue(Queue *q);      // Entrega verdadero si q esta vacia, falso si no
int queueLength(Queue *q);     // Entrega el tamano actual de q

//==========================================
// Funciones para el TDA sort generico
//==========================================

// sortPtrArray(arr, i, j, compare) ordena el arreglo de punteros arr desde la
// posicion i a la j (inclusives) de acuerdo al criterio especificado
// por la funcion de comparacion compare.  La funcion de comparacion
// debe tener este encabezado:
//   int compare(void *arr, int i, int j);
// y debe entregar < 0 si arr[i] es menor que arr[j],
//                   0   si a[i] es igual a a[j]
//               o > 0 si no.
//
// Por ejemplo para ordenar por edad el arreglo de personas definidas
// por el tipo Persona (ver archivo ejemplo-sort.c):
//
//   typedef struct {
//     char *nom;
//     int edad;
//   } Persona;
//
// la funcion de comparacion debe ser:
//
//   int cmpPersonas(void *arr, int i, int j) {
//     Persona **a= ptr;
//     return a[i]->edad - a[j]->edad;
//   }
//
// Luego para ordenar el arreglo ppersonas definido a continuacion como:
//
//   Persona pedro= {"pedro", 5}, juan={"juan", 10}, diego{"diego", 3};
//   Persona *ppersonas[ ]= { &pedro, &juan, &diego };
//
// se debe invocar:
//   sortPtrArray(ppersonas, 0, 2, cmpPersonas);
//
// Importante: sortPtrArray solo puede ordenar arreglos de punteros.
// Por ejemplo permite ordenar el arreglo personas definido como:
//   Persona *ppersonas[10];
// pero no sirve para ordenar un arreglo definido como:
//   Persona persona[10];
// porque este no seria un arreglo de punteros.

typedef int (*Comparator)(void *ptr, int i, int j);
void sortPtrArray(void *ptr, int left, int right, Comparator compare);

// Para ordenar arreglos generales debe usar la funcion sort.
// En tal caso, ademas de la funcion de comparacion debe proveer la funcion
// que intercambia los elementos en el arreglo (ver ejemplo-sort.c).

typedef void (*Swapper)(void *ptr, int i, int j);
void sort(void *ptr, int left, int right, Comparator compare, Swapper swap);
