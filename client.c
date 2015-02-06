#include <stdio.h>
#include <time.h>
#include <string.h>
#include <winsock2.h>
#include "TCPclient.h"


#define TIMEOUT 1*CLOCKS_PER_SEC

int main(int argc, char* argv[])
{
 char buffer[1024] = "REQ\r\n"; // stringa di richiesta
 unsigned long start, now;
 unsigned int *num = (unsigned int*)buffer;
 int i, n;

 if (argc<2)
	 {
		printf("Deve essere fornito l’indirizzo IP del server!\r\n");
		return -1;
	 }
 // connessione al server (numero di porta 23365)
 if (TCPclient_connect(argv[1], 23365) < 0)
	 {
		printf("Errore connessione server!\r\n");
		return -1;
	 }
 // trasmissione richiesta al server
 TCPclient_send(buffer, strlen(buffer));
 i = 0;
 start = clock(); // tempo iniziale attesa
 now = clock(); // tempo attuale
 // ciclo di attesa risposta per al massimo un tempo pari a TIMEOUT
 while ((now - start) < TIMEOUT)
			{
			 if ((n = TCPclient_receive(&buffer[i], sizeof(buffer)-i)) > 0)
				 {
					// ricevuti caratteri: ricostruzione risposta
					i += n;
					if (i >= sizeof(unsigned int))
						{
						 // risposta completa
						 printf("Ricevuto numero %u.\r\n", ntohl(*num));
						 TCPclient_disconnect();
						 return 0;
						}
					}
				now = clock(); // aggiornamento tempo attuale
			 }
 // trascorso un tempo pari a TIMEOUT senza completamento risposta
 printf("Nessuna risposta ricevuta!\r\n");
 TCPclient_disconnect();
 return -1;
}
