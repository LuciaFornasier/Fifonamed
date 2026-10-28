#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

#define FIFO_PATH "/tmp/mia_fifo"
#define BUF_SIZE  256

int main() {

    if (mkfifo(FIFO_PATH, 0666) == -1 && errno != EEXIST) {
        perror("Errore mkfifo");
        exit(EXIT_FAILURE);
    }

    printf("[Server] Attendo un client...\n");
    fflush(stdout);

    int fd = open(FIFO_PATH, O_RDONLY);
    if (fd == -1) {
        perror("Errore open server");
        exit(EXIT_FAILURE);
    }

    printf("[Server] Client connesso. Lettura in corso...\n");

    char buffer[BUF_SIZE];

    while (1) {
        ssize_t n = read(fd, buffer, sizeof(buffer) - 1);

        if (n > 0) {
            buffer[n] = '\0';

            if (buffer[0] == 'Q' && buffer[1] == 'U' && buffer[2] == 'I' && buffer[3] == 'T') {
                printf("[Server] Ricevuto QUIT. Arrivederci!\n");
                break;
            }

            printf("[SERVER] %s", buffer);
            if (buffer[n - 1] != '\n')
                printf("\n");
        } else if (n == 0) {
            printf("[Server] Il client ha chiuso senza inviare QUIT.\n");
            break;
        } else {
            perror("Errore read");
            break;
        }
    }

    close(fd);
    if (unlink(FIFO_PATH) == -1)
        perror("Errore unlink");
    else
        printf("[Server] FIFO rimossa dal filesystem.\n");

    return 0;
}