#include <stdio.h>
#include <stdlib.h>

#include "provider.h"
#include "accepter.h"
#include "reader_list.h"
#include "dispatcher.h"
#include <pthread.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

void create_requester(pthread_t requester[], int num) {
    int i;
    char cont[20];
    msg_t* message;
    for (i = 0; i < num; i++) {
        sprintf(cont, "Reader %d", i);
        message = msg_init_string((void*) cont);
        pthread_create(&requester[i], NULL, accepter_buffer_insert, (void*) message);
        pthread_join(requester[i], NULL);
        msg_destroy_string(message);
    }

    sleep(1);
}

main() {
    pthread_t provider;
    pthread_t accepter;
    pthread_t dispatcher;

    msgs* content;
    int dim = 0;
    int str = 0;
    int req = 0;
    int i;
    printf("Che dimensione vuoi assegnare al buffer?\n");
    scanf("%d", &dim);
    printf("Init buffer\n");
    provider_buffer_init(dim);
    accepter_buffer_initialize(dim);
    sleep(1);



    printf("Che dimensione vuoi assegnare allo streaming?\n");
    scanf("%d", &str);
    char cont[20];
    content = (msgs*) calloc(str, sizeof (msgs));
    for (i = 0; i < str; i++) {
        sprintf(cont, "Messaggio %d", i);
        content->messages[i] = msg_init_string(cont);
    }
    content->size = str;
    printf("Provider init\n");
    pthread_create(&provider, NULL, create_messages_streaming, (void*) content);



    printf("Accepter init\n");
    pthread_create(&accepter, NULL, submitRequest, NULL);

    printf("Quanti reader vuoi aggiungere?\n");
    scanf("%d", &req);
    pthread_t requester[req];
    printf("Invio delle richieste\n");
    create_requester(requester, req);


    printf("Dispatcher init\n");
    pthread_create(&dispatcher, NULL, dispatch_message, NULL);
    sleep(1);
    pthread_join(provider, NULL); //mi serve per aspettare la fine del provider
    printf("\nprovider die");
    accepter_buffer_insert((void*) &POISON_PILL_MSG);
    pthread_join(accepter, NULL);
    pthread_join(dispatcher, NULL);


    printf("Delete buffer\n");
    provider_buffer_destroy();
    accepter_buffer_destroy();
    printf("Exit\n");
}
