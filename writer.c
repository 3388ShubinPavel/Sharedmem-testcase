#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define SHM_SIZE 1000 
#define KEY_FILE "shm_key.txt" 

int main() {
    key_t key;
    int shmid;
    char *data;
    const char *filename = "shmfile"; 

    if (access(filename, F_OK) != 0) {
        int fd = open(filename, O_CREAT | O_RDWR, 0666);
        if (fd == -1) {
            fprintf(stderr, "Error creating file %s: %s (errno=%d)\n", filename, strerror(errno), errno);
            return 1;
        }
        close(fd);
    }

    if ((key = ftok(filename, 'R')) == -1) {
        fprintf(stderr, "Error in ftok: %s (errno=%d)\n", strerror(errno), errno);
        return 1;
    }

    printf("Shared memory key: %d\n", key);

    if ((shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666)) == -1) {
        fprintf(stderr, "Error in shmget: %s (errno=%d)\n", strerror(errno), errno);
        return 1;
    }

    if ((data = (char *)shmat(shmid, NULL, 0)) == (char *)(-1)) {
        fprintf(stderr, "Error in shmat: %s (errno=%d)\n", strerror(errno), errno);
        shmctl(shmid, IPC_RMID, NULL); 
        return 1;
    }

    strcpy(data, "Hello world!");

    if (shmdt(data) == -1) {
        fprintf(stderr, "Error in shmdt: %s (errno=%d)\n", strerror(errno), errno);
        return 1;
    }

    FILE *key_file = fopen(KEY_FILE, "w");
    if (!key_file) {
        fprintf(stderr, "Error creating key file: %s (errno=%d)\n", strerror(errno), errno);
        return 1;
    }
    fprintf(key_file, "%d", key); 
    fclose(key_file);

    return 0;
}