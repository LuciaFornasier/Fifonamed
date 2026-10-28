#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define FIFO_PATH "/tmp/mia_fifo"
#define BUF_SIZE  256

int main(void) {
    char buffer[BUF_SIZE];

    printf("[Client] Inserisci una riga: ");
    fflush(stdout);
    if (fgets(buffer, BUF_SIZE, stdin) == NULL) {
        fprintf(stderr, "[Client] Nessun input.\n");
        exit(EXIT_FAILURE);
    }

    if (mkfifo(FIFO_PATH, 0666) == -1 && errno != EEXIST) {
        perror("Errore mkfifo");
        exit(EXIT_FAILURE);
    }

    printf("[Client] Attendo il server...\n");
    fflush(stdout);

    int fd = open(FIFO_PATH, O_WRONLY);
    if (fd == -1) {
        perror("Errore open client");
        exit(EXIT_FAILURE);
    }

    if (write(fd, buffer, strlen(buffer)) == -1) {
        perror("Errore write");
        close(fd);
        exit(EXIT_FAILURE);
    }

    close(fd);

    return 0;
}