// ex7
// Haokun Cai
// Haokun@uw.edu
// 1832787
// 07/11/20
// Copyright[2019]<Haokun Cai>

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <dirent.h>
#include <limits.h>
#include <stddef.h>
#include <string.h>

#define buf_size 1024  // size of buffer

void PrintText(char* filename);

void FullPath(char *directory, char *file);

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("wrong argument number\n");
        return EXIT_FAILURE;
    }

    DIR *dir = opendir(argv[1]);
    if (dir == NULL) {
        printf("missing directory\n");
        return EXIT_FAILURE;
    }

    struct dirent* entry = readdir(dir);
    while (entry != NULL) {
        int len = strlen(entry->d_name);
        if (len >= 4 && strcmp(entry->d_name + (len - 4), ".txt") == 0) {
            FullPath(argv[1], (char*)entry->d_name);
        }
        entry = readdir(dir);
    }
    closedir(dir);
    return EXIT_SUCCESS;
}

void FullPath(char *dir, char *file) {
  int len = strlen(dir) + strlen(file);
  char *full_path= (char *) malloc(sizeof(char) * (len + 2));
  if (full_path == NULL) {
    fprintf(stderr, "Error on malloc.\n");
  }
  snprintf(full_path, len + 2, "%s/%s", dir, file);

  PrintText(full_path);
  free(full_path);
}

void PrintText(char* filename) {
    int file = open(filename,  O_RDONLY);
    if (file == -1) {
        perror("can't open file");
        exit(EXIT_FAILURE);
    }

    char buf[1024];
    while (1) {
        int result = read(file, buf, buf_size);
        if (result == 0) {
            break;
        } else if (result == -1) {
            if (errno != EINTR) {
            perror("read failed");
            exit(EXIT_FAILURE);
            }
        continue;
        }
        if (fwrite(buf, result, 1, stdout) != 1) {
        fprintf(stderr, "fwrite failed\n");
        }
    }
    close(file);
}
