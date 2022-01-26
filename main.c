#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//CLionProjects\arc\cmake-build-debug\arc.exe --file data.arc --create a.txt b.txt c.txt
//CLionProjects\arc\cmake-build-debug\arc.exe --file data.arc --list
//CLionProjects\arc\cmake-build-debug\arc.exe --file data.arc --extract

int MAX_FILE_SIZE = 100;

int find_end_of_name(char * name_string, int size) {
    int x = 0;
    for (int i = size - 1; i >= 0; i--) {
        if (name_string[i] == '-') {
            x = i;
            break;
        }
    }
    return x;
}

long int find_size(char *current_file) {
    FILE *f = fopen(current_file, "r+");
    int size = 0;
    while (!feof(f)) {
        getc(f);
        size++;
    }
    fclose(f);
    return size;
}

void create (char *arc_name, char *add_file_name) {
    FILE *arc = fopen(arc_name, "a+");
    FILE *file_for_read = fopen(add_file_name, "r+");
    fprintf(arc, "%s-%ld\n", add_file_name, find_size(add_file_name));
    while (!feof(file_for_read)) {
        int sym = fgetc(file_for_read);
        fputc(sym, arc);
    }
    fclose(arc);
    fclose(file_for_read);
}

void list(char *arc_name) {
    FILE *arc = fopen(arc_name, "r+");
    int flag = 1;
    while (!feof(arc)) {
        int sym = fgetc(arc);
        if (flag == 1) {
            printf("%c", sym);
        }
        if (sym == 10 && flag == 1) {
            flag = 0;
        }
        if (sym == 255 && flag == 0) {
            flag = 1;
        }
    }
}

void extract(char *arc_name) {
    FILE *arc = fopen(arc_name, "r+");
    int flag = 1;
    char head[MAX_FILE_SIZE];
    int current_size = 0;
    while (!feof(arc)) {
        while (flag == 1) {
            int sym = getc(arc);
            head[current_size] = sym;
            current_size++;
            if (sym == 10) {
                flag = 0;
            }
        }
        char file_name[MAX_FILE_SIZE];
        int pos = find_end_of_name(head, current_size);
        for (int i = 0; i < pos; i++) {
            file_name[i] = head[i];
        }
        file_name[pos] = 0;
        char file_size[MAX_FILE_SIZE];
        for (int i = pos + 1; i < current_size - 1; i++) {
            file_size[i - pos - 1] = head[i];
        }
        int sum = atoi(file_size);
        FILE *file = fopen(file_name, "w+");
        for (int i = 1; i < sum; i++) {
            int sym = getc(arc);
            fputc(sym, file);
        }
        fclose(file);
        int sym = getc(arc);
        if (sym == 255) {
            flag = 1;
        }
        current_size = 0;
    }
}

int main(int argc, char* argv[]) {
    char *archiva_name;
    if (strcmp(argv[1], "--file") == 0) {
        archiva_name = argv[2];
        if (strcmp(argv[3], "--list") == 0) {
            list(archiva_name);
        }
        else if (strcmp(argv[3], "--extract") == 0) {
            extract(archiva_name);
        }
        else if (strcmp(argv[3], "--create") == 0) {
            for (int i = 4; i < argc; i++) {
                create(archiva_name, argv[i]);
            }
        }
    }
    return 0;
}
