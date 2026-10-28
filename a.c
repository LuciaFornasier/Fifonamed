#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define FIFO1 "/tmp/fifo_a_to_b"
#define FIFO2 "/tmp/fifo_b_to_a"
#define BUF_SIZE 256

int main(void) {
    char buffer[BUF_SIZE];

    if (mkfifo(FIFO1, 0666) == -1 && errno != EEXIST) {
        perror("Errore mkfifo FIFO1");
        exit(EXIT_FAILURE);
    }
    if (mkfifo(FIFO2, 0666) == -1 && errno != EEXIST) {
        perror("Errore mkfifo FIFO2");
        exit(EXIT_FAILURE);
    }

    printf("[A] Attendo B...\n");
    fflush(stdout);

    int fd_out = open(FIFO1, O_WRONLY);
    if (fd_out == -1) {
        perror("Errore open FIFO1");
        exit(EXIT_FAILURE);
    }
    int fd_in = open(FIFO2, O_RDONLY);
    if (fd_in == -1) {
        perror("Errore open FIFO2");
        exit(EXIT_FAILURE);
    }

    printf("[A] Connesso.\n");

    while (1) {
        printf("[A] Scrivi (EXIT per uscire): ");
        fflush(stdout);

        if (fgets(buffer, BUF_SIZE, stdin) == NULL)
            break;

        if (write(fd_out, buffer, strlen(buffer)) == -1) {
            perror("Errore write");
            break;
        }

        if (buffer[0] == 'E' && buffer[1] == 'X' && buffer[2] == 'I' && buffer[3] == 'T')
            break;

        printf("[A] Attendo la risposta di B...\n");
        ssize_t n = read(fd_in, buffer, BUF_SIZE - 1);

        if (n <= 0) {
            printf("[A] B ha chiuso la comunicazione.\n");
            break;
        }
        buffer[n] = '\0';
        printf("[A] B dice: %s", buffer);

        if (buffer[0] == 'E' && buffer[1] == 'X' && buffer[2] == 'I' && buffer[3] == 'T') {
            printf("[A] B ha inviato EXIT.\n");
            break;
        }
    }

    close(fd_out);
    close(fd_in);
    unlink(FIFO1);
    unlink(FIFO2);
    printf("[A] Fine.\n");

    return 0;
}
