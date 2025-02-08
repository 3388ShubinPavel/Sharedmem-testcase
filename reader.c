#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h> 
#include <unistd.h> 

#define SHM_SIZE 1000 
#define KEY_FILE "shm_key.txt" 

int main() {
    key_t key;
    int shmid;
    char *data;

    FILE *key_file = fopen(KEY_FILE, "r");
    if (!key_file) {
        fprintf(stderr, "Error opening key file: %s (errno=%d)\n", strerror(errno), errno);
        return 1;
    }
    if (fscanf(key_file, "%d", &key) != 1) {
        fprintf(stderr, "Error reading key from file\n");
        fclose(key_file);
        return 1;
    }
    fclose(key_file);

    printf("Read shared memory key: %d\n", key);

    if ((shmid = shmget(key, SHM_SIZE, 0666)) == -1) {
        fprintf(stderr, "Error in shmget: %s (errno=%d)\n", strerror(errno), errno);
        return 1;
    }

    if ((data = (char *)shmat(shmid, NULL, 0)) == (char *)(-1)) {
        fprintf(stderr, "Error in shmat: %s (errno=%d)\n", strerror(errno), errno);
        return 1;
    }

    printf("Data read from shared memory: %s\n", data);

    if (shmdt(data) == -1) {
        fprintf(stderr, "Error in shmdt: %s (errno=%d)\n", strerror(errno), errno);
        return 1;
    }

    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        fprintf(stderr, "Error in shmctl: %s (errno=%d)\n", strerror(errno), errno);
        return 1;
    }

    if (remove(KEY_FILE) != 0) {
        fprintf(stderr, "Error removing key file: %s (errno=%d)\n", strerror(errno), errno);
    }

    return 0;
}