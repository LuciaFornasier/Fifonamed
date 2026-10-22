#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>       /* per open() e O_RDONLY */
#include <sys/stat.h>    /* per mkfifo() */
#include <unistd.h>      /* per read(), close(), unlink() */

#define FIFO_PATH "/tmp/mia_fifo"
#define BUF_SIZE  256

int main() {
    printf("[Reader] Attendo un writer...\n");
    fflush(stdout);

    /* 1. Apre in sola lettura -- SI BLOCCA finche' non c'e' un writer */
    int fd = open(FIFO_PATH, O_RDONLY);
    if (fd == -1) {
        perror("Errore open reader");
        exit(EXIT_FAILURE);
    }

    printf("[Reader] Writer connesso. Lettura in corso...\n");

    /* 2. Legge i dati dalla FIFO */
    char buffer[BUF_SIZE];
    ssize_t n = read(fd, buffer, sizeof(buffer));

    if (n > 0)
        printf("[Reader] Ricevuto: %s\n", buffer);
    else if (n == 0)
        printf("[Reader] Il writer ha chiuso senza inviare dati.\n");
    else
        perror("Errore read");

    /* 3. Chiude il file descriptor */
    close(fd);

    /* 4. Rimuove la FIFO dal filesystem */
    if (unlink(FIFO_PATH) == -1)
        perror("Errore unlink");
    else
        printf("[Reader] FIFO rimossa dal filesystem.\n");

    return 0;
}