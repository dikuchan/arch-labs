#ifndef PATTERN_H
#define PATTERN_H

#include <stdio.h>
#include <stdbool.h>
#include <omp.h>

#include "utils.h"

bool match(char* string, char* mask)
{
    char* rs = 0, // Place to return in string if `*` didn't match
        * rm;     // Place to return in mask

    for (;;) {
        if (*mask == '*') {
            rs = string;
            rm = ++mask;
        }
        else if (!*string) { return !*mask; }
        else if (*string == *mask || *mask == '?') {
            ++string;
            ++mask;
        }
        else if (rs) {
            string = ++rs;
            mask = rm;
        }
        else { return false; }
    }
}

bool search(char** text, ull lines, char* mask)
{
    {
        bool found = false, lc = false;
        ull i = 0;
#pragma omp parallel for private(i) reduction(||: found) num_threads(lines)
        iterate(, i, lines) {
            lc = match(text[i], mask);
            if (lc) { printf("\t\t%llu\n", i); }
            found |= lc;
        }

        return found;
    }
}

ull pattern(const char* filename, char* mask)
{
    timeval start, end;

    FILE* file;
    ull lines = 0;

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

    rewind(file);

    iterate(ull, i, lines) {
        char* line = (char*) malloc(sizeof(char) * 1024);
        fscanf(file, "%s", line); // Read line to buffer
        text[i] = line; // Save it
    }

    /**
     * Algorithm
     */

    gettimeofday(&start, NULL);

    printf("\tString(s) with the pattern found on line(s):\n");
    if (!search(text, lines, mask)) { printf("\t\tNone\n"); }

    gettimeofday(&end, NULL);

    fclose(file); // Close file
    iterate(ull, i, lines) { free(text[i]); } // Deallocate lines
    free(text); // Deallocate buffer

    return ELAPSED;
}

#endif // PATTERN_H
