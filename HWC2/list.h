/*
 * Autore: Valter Crescenzi e gli studenti del 
 * Corso di Programmazione Concorrente
 *  
 * list.h
 *
 * Liste come da specifiche di HWC2
 */

#ifndef LIST_H_
#define LIST_H_

#include <pthread.h>

//nodo di lista
typedef struct list {
	void* payload;	   // oggetto nella lista
	struct list* next; // elemento successivo
} list_node_t;

/* lista semplicemente collegata di strutture list_node_t
 * Gli iteratori conoscono la lista, ma la lista non conosce
 * i suoi iteratori. Alcuni controlli relativi omessi per semplicità
 */
typedef struct {
	list_node_t* head;
	list_node_t* tail;
} list_t;

// iterator
typedef struct {
	list_t* list;
	list_node_t* currentNode;
} iterator_t;

// Gli elementi contengono generici puntatori a void (void *)

//N.B. Gli elementi non possono essere NULL
list_t* list_init();             // crea una lista vuota
void list_destroy(list_t* list); // dealloca la struttura lista (non i contenuti)
int size(list_t* list);          // restituisce il numero di elementi
int isEmpty(list_t* list);       // restituisce vero sse vuota

void  addElement(list_t* list, void* element);  // aggiunge in fondo un elemento
int removeElement(list_t* list, void* element); // rimuove un elemento dalla lista 
// si sceglie l'elemento di indirizzo element; restituisce false se inesistente
iterator_t* iterator_init(list_t* list); // crea un iteratore sulla lista
void iterator_destroy(iterator_t* it); // dealloca un iteratore
int hasNext(iterator_t* it); // iterazione finita?
void *next(iterator_t* it);  // prossimo elemento, NULL se finiti


#endif /* LIST_H_ */