#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>       /* per open() e O_WRONLY */
#include <sys/stat.h>    /* per mkfifo() */
#include <unistd.h>      /* per write() e close() */
#include <string.h>      /* per strlen() */
#include <errno.h>       /* per errno e EEXIST */

#define FIFO_PATH "/tmp/mia_fifo"

int main() {
    /* 1. Crea la FIFO nel filesystem */
    if (mkfifo(FIFO_PATH, 0666) == -1) {
        if (errno != EEXIST) {
            perror("Errore mkfifo");
            exit(EXIT_FAILURE);
        }
    }

    printf("[Writer] FIFO pronta. Attendo un reader...\n");
    fflush(stdout);   /* forza la stampa prima del blocco */

    /* 2. Apre in sola scrittura -- SI BLOCCA finche' non c'e' un reader */
    int fd = open(FIFO_PATH, O_WRONLY);
    if (fd == -1) {
        perror("Errore open writer");
        exit(EXIT_FAILURE);
    }

    printf("[Writer] Reader connesso. Invio messaggio...\n");

    /* 3. Scrive il messaggio nella FIFO (terminatore incluso) */
    const char *msg = "Ciao dal processo Writer!";
    write(fd, msg, strlen(msg) + 1);

    /* 4. Chiude il file descriptor */
    close(fd);
    printf("[Writer] Messaggio inviato. Canale chiuso.\n");

    return 0;
}