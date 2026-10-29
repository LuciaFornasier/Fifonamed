/*Scrivi un programma che legge comandi da tastiera in un ciclo (un comando per riga, senza argomenti),
e per ognuno esegue il pattern fork + execvp.
 Il programma termina quando l'utente inserisce exit.
 Il padre aspetta sempre che il figlio finisca prima di chiedere il prossimo comando.
 Se il comando non esiste, stampa un messaggio di errore e continua.
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
 
int main() {
    printf("[Padre] PID: %d -- sto per fare fork()\n", getpid());
 
    pid_t pid = fork();
 
    if (pid == -1) {
        perror("Errore fork");
        exit(EXIT_FAILURE);
    }
 
    if (pid == 0) {
        printf("[Figlio] PID: %d -- chiamo execvp()\n", getpid());
        char *args[] = { "ls", "-l", "/tmp", NULL };
        execvp("ls", args);
 

        perror("Errore execvp");
        exit(EXIT_FAILURE);
    }


    int status;
    wait(&status);
 
    if (WIFEXITED(status))
        printf("[Padre] figlio terminato con codice %d\n",
               WEXITSTATUS(status));
 
    printf("[Padre] continuo la mia esecuzione.\n");
    return 0;
}

