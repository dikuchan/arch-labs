#include <stdio.h>
#include <stdbool.h>
#include <omp.h>

#include "utils.h"

bool match(char* string, char* pattern)
{
    char* rs = 0, * rp;

    for (;;) {
        if (*pattern == '*') {
            rs = string;
            rp = ++pattern;
        }
        else if (!*string) { return !*pattern; }
        else if (*string == *pattern || *pattern == '?') {
            ++string;
            ++pattern;
        }
        else if (rs) {
            string = ++rs;
            pattern = rp;
        }
        else { return false; }
    }
}

void search(char** buffer, ull lines, char* pattern)
{
    {
        bool found = false;
        ull i = 0;
#pragma omp parallel for private(i) reduction(||: found) num_threads(lines)
        iterate(, i, lines) {
            found |= match(buffer[i], pattern);
        }

        if (found) { printf("String is present in text"); }
    }
}

int main()
{
#if defined(INLINE)
    const char* filename = "sample.txt";
    char* pattern = "*a";
#else
    char filename[128], pattern[128];

    if (!fgets(filename, 128, stdin)) {
        perror("Reading error");
        return 1;
    }
    if (!fgets(pattern, 128, stdin)) {
        perror("Reading error");
        return 1;
    }
#endif

    FILE* file;
    ull length, lines = 0;

    file = fopen(filename, "rb");
    if (!file) {
        perror("File error");
        return 1;
    }

    while (!feof(file)) {
        char character = fgetc(file);
        if (character == '\n') {
            lines += 1;
        }
    }
    char** text = (char**) malloc(sizeof(char*) * lines);

    fseek(file, 0, SEEK_END);
    length = ftell(file);
    rewind(file);

    iterate(ull, i, lines) {
        char* line = (char*) malloc(sizeof(char) * 1024);
        fscanf(file, "%s", line);
        text[i] = line;
    }

    search(text, lines, pattern);

    fclose(file);

    iterate(ull, i, lines) { free(text[i]); }
    free(text);

    return 0;
}
