#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

#define FIFO_PATH "/tmp/mia_fifo"
#define BUF_SIZE  256

int main(void) {
    char buffer[BUF_SIZE];

    signal(SIGPIPE, SIG_IGN);

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

    while (1) {
        printf("[Client] Inserisci una riga (QUIT per uscire): ");
        fflush(stdout);

        if (fgets(buffer, BUF_SIZE, stdin) == NULL) {
            fprintf(stderr, "[Client] Nessun input.\n");
            break;
        }

        if (write(fd, buffer, strlen(buffer)) == -1) {
            perror("Errore write");
            close(fd);
            exit(EXIT_FAILURE);
        }

        if (buffer[0] == 'Q' && buffer[1] == 'U' && buffer[2] == 'I' && buffer[3] == 'T')
            break;
    }

    close(fd);

    return 0;
}