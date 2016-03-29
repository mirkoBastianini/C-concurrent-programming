/*
 * Autore: Valter Crescenzi e gli studenti del 
 * Corso di Programmazione Concorrente
 *
 * TestDriver.c
 */


#include <stdlib.h>
#include <stdio.h>

#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#include "test/testList.h"
#include "test/testProvider.h"
#include "test/testReader.h"
#include "test/testReaderList.h"
#include "test/testAccepter.h"
#include "test/testDispatcher.h"


int main(int argc, char** argv){

	 
	    CU_ErrorCode ret = CU_initialize_registry();
	    if(ret==CUE_SUCCESS)
	        printf("inizio dei test\n");
	    if(ret==CUE_NOMEMORY){
	        printf("ERRORE NEI TEST\n");
	        exit(-1);
	    }

	    CU_TestInfo suiteListe[]= {
	            { "Creazione", test_list_init},
	            { "Eliminazione lista vuota", test_list_destroy_emptyList},
	            { "Eliminazione lista con 1 elemento", test_list_destroy_singletonList},
	            { "Dimensione di una lista vuota", test_list_size_emptyList},
	            { "Dimensione di una lista non vuota", test_list_size_nonEmptyList},
	            { "Lista vuota? Si.", test_list_isEmpty_emptyList},
	            { "Lista vuota? No.", test_list_isEmpty_nonEmptyList},
	            { "Dimensione di una lista non vuota", test_list_size_nonEmptyList},
	            { "Aggiunta 1 elemento, lista vuota", test_addElement_emptyList},
	            { "Aggiunta 2 elementi", test_addElement_2elements},
	            { "Aggiunta di piu\' di 2 elementi", test_addElement_3elements},
	            { "Aggiunta/Eliminazione dell'unico elemento", test_addAndRemoveElement},
	            { "Eliminazione del primo di due elementi", test_removeElement_2elements_lastRemain},
	            { "Eliminazione del secondo di due elementi", test_removeElement_2elements_firstRemain},
	            { "Svuotamento di lista di 2 elementi", test_removeElement_2elements_empties},
	            { "Eliminazione del primo di tre elementi", test_removeElement_3elements_first},
	            { "Eliminazione del secondo di tre elementi", test_removeElement_3elements_middle},
	            { "Eliminazione del terzo di tre elementi", test_removeElement_3elements_last},
	            { "Svuotamento di una lista di 3 elementi", test_removeElement_all},
	            { "Creazione/Distruzione di un iteratore", test_iterator_init_destroy},
	            { "Iteratore: hasNext su lista vuota", test_hasNext_emptyList},
	            { "Iteratore: hasNext su lista non vuota", test_hasNext_nonEmptyList},
	            { "Iteratore: next su lista vuota", test_next_emptyList},
	            { "Iteratore: iterazione su tutta una lista ", test_next_nonEmptyList},
	            CU_TEST_INFO_NULL
	        };
            
            CU_TestInfo suiteProvider[]={
			{"Inserimento di un solo messaggio nel buffer del Provider", test_provider_buffer_insert_oneMessage},
			{"Inserimento di otto messaggi nel buffer del Provider", test_provider_buffer_insert_eightMessage},
			{"Emissione dei messaggi da parte del Provider", test_createMessageSequence},
			{"Inserimento e estrazione di un solo messaggio nel buffer del Provider", test_provider_buffer_insertAndRead},
	        CU_TEST_INFO_NULL
		};
            
            CU_TestInfo suiteReaderBuffer[]={
			{"Inserimento di un solo messaggio", test_reader_insert_oneMessage},
			{"Inserimento di otto messaggi", test_reader_insert_eightMessage},
			CU_TEST_INFO_NULL
		};

            
		CU_TestInfo suiteReader[]={
			{"Funzionamento del Reader - consumo dei messaggi", test_reader_consume},
	        CU_TEST_INFO_NULL
		};

		CU_TestInfo suiteReaderList[]={
			{"Verifica che la lista dei Reader sia vuota", test_reader_list_isEmpty},
			{"Aggiunta di un Reader alla lista", test_reader_list_addOneReader},
			{"Verifica che la lista dei Reader non sia vuota", test_reader_list_isNotEmpty},
			{"Aggiunta di due Reader alla lista", test_reader_list_addTwoReaders},
			{"Verifica l'invio della POISON PILL a tutti i Reader della lista", test_reader_list_kill},
			{"Verifica l'invio di un messaggio a tutti i Reader della lista", test_reader_list_broadcast},
			CU_TEST_INFO_NULL
		};

		CU_TestInfo suiteAccepter[]={
			{"Inserimento di un solo messaggio nel buffer dell'Accepter", test_accepter_buffer_insert_oneMessage},
			{"Inserimento di otto messaggi nel buffer dell'Accepter", test_accepter_buffer_insert_eightMessage},
			{"Recupero di un messaggio dal buffer dell'Accepter", test_accepter_buffer_delete},
			{"Eliminazione di tutti i messaggi dal buffer dell'Accepter", test_accepter_buffer_deleteAll},
			{"Sottomissione di una richiesta da parte dell'Accepter", test_accepter_submitRequestNormal},
                        {"Sottomissione di una poisonPill da parte dell'Accepter", test_accepter_submitRequestPoisonPill},
	        CU_TEST_INFO_NULL
		};

		CU_TestInfo suiteDispatcher[]={
			{"Smistamento dei messaggi da parte del Dispatcher", test_dispatch_message},
			{"Eliminazione dei reader lenti da parte del Dispatcher", test_dispatchAndDelete_slow_reader},
	        CU_TEST_INFO_NULL
		};

	    CU_SuiteInfo suites[] = {
               {"Test sulle liste", listBefore, listAfter, suiteListe},
	       {"Test del funzionamento del Provider", provider_buffer_before,provider_buffer_after,suiteProvider},
	       {"Test del funzionamento del buffer del Reader", reader_before1,reader_after1,suiteReaderBuffer},
               {"Test del funzionamento del Reader", reader_before2,reader_after2,suiteReader},
               {"Test del funzionamento della lista dei Reader", reader_list_before,reader_list_after,suiteReaderList},
	       {"Test del funzionamento dell'Accepter", accepter_buffer_before,accepter_buffer_after,suiteAccepter},
	       {"Test del funzionamento del Dispatcher", dispatcher_before,dispatcher_after,suiteDispatcher},
	        CU_SUITE_INFO_NULL
	    };
	    

	    CU_ErrorCode error = CU_register_suites(suites);
	    if(error != CUE_SUCCESS){
	        printf("Errore con CUnit - codice %d\n", error);
	    }

	    //far girare i test - modo base, risultato su stdout
	    CU_basic_set_mode(CU_BRM_VERBOSE);
	        //CU_BRM_NORMAL 	Failures and run summary are printed.
	        //CU_BRM_SILENT 	No output is printed except error messages.
	        //CU_BRM_VERBOSE 	Maximum output of run details.
	    CU_ErrorCode tuttoOk = CU_basic_run_tests();
	    printf("CUnit error code-->%d\n", tuttoOk);

	    //lettura risultati
	    unsigned int failures = CU_get_number_of_failures();
	    if(failures == 0)
	        printf("Barra Verde!\n");
	    else
	        printf("%d fallimenti\n",CU_get_number_of_failures());

	    //alla fine, pulire. UNA SOLA volta
	    CU_cleanup_registry();

	    return 0;
}

