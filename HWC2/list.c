/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
/*
 * Autore: Valter Crescenzi e gli studenti del 
 * Corso di Programmazione Concorrente
 *  
 * list.c
 *
 * Implementa list.h
 */


#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include "list.h"

/************* FUNZIONI PRIVATE ***********************/
list_node_t* list_node_init(void* payload){
  list_node_t* result = malloc(sizeof(list_node_t));
  result->next=NULL;
  result->payload=payload;
  return result;
}

/*************FUNZIONI DELL'INTERFACCIA***************/

list_t* list_init() {
  list_t* result = malloc(sizeof(list_t));
  result->head=NULL;
  result->tail=NULL;
  return result;
}


void list_destroy(list_t *list) {
  if (list==NULL) { 
    perror("tentato il free su una lista già NULL");
    exit(-1);
  }

  //elimina tutti gli elementi/nodi MA NON e il loro contenuto/payload
  list_node_t* current = list->head;
  list_node_t* toBeFreed;
  while (current != NULL) {
    toBeFreed=current;
    current=current->next;
    free(toBeFreed);
  }

  free(list);
}

int size(list_t *list) {
  int result=0;
  list_node_t* current = list->head;
  while (current != NULL ) {
    current=current->next;
    result++;
  }
  return result;
}

int isEmpty(list_t *list) {
  return ( list->head==NULL );
}

void addElement(list_t *list, void *element) {
  if (element==NULL) {  // non sono ammessi valori nulli come elementi
   perror("tentato di aggiungere un valore NULL");
    exit(-1);
  }

  //creo un nodo
  list_node_t* newNode=list_node_init(element);

  //lista vuota?
  if (list->head==NULL) {
    list->head=newNode;
    list->tail=newNode;
  } else {
    // lista non vuota
    list->tail->next=newNode;
    list->tail=newNode;
  }
}


/* 
 * Rimuovere l'elemento il cui payload ha indirizzo element.
 * Rimuove l'elemento dalla lista, non dealloca il payload 
*/
int removeElement(list_t *list, void *element) {
  // ricerca nodo il cui contenuto ha indirizzo elemento
  list_node_t* current = list->head;
  list_node_t* previous=NULL;
  while (current != NULL && current->payload != element ){
    previous=current;
    current=current->next;
  }

  // elemento non trovato?
  if (current==NULL) {
    return 0;
  } else if (current==list->head) { // era il primo?
    list->head=current->next;
  }

  //era uno qualsiasi?
  if (previous != NULL)
    previous->next=current->next;

  //e' cambiata la coda?
  if (current==list->tail)
    list->tail=previous;

  free(current);
  return 1;
}

iterator_t* iterator_init(list_t *list) {
  iterator_t* result = malloc(sizeof(iterator_t));
  result->currentNode=list->head;
  result->list=list;
  return result;
}

void iterator_destroy(iterator_t *it) {
  // la lista vive più a lungo dell'iteratore
  free(it);
}

int hasNext(iterator_t *it) {
  return ( it->currentNode != NULL );
}

void *next(iterator_t *it) {
  void* result = NULL;
  if ( it->currentNode!=NULL ) {
    result = it->currentNode->payload;
    it->currentNode=it->currentNode->next;
  }
  return result;
}

