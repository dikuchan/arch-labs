#ifndef MATRIX_H
#define MATRIX_H

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "utils.h"

/**
 * #define INLINE : If defined, does not require user input
 * #define WRITE  : If defined, writes matrix to file
 * #define RMAX   : Random generator range
 */

#ifndef RMAX
#define RMAX 16384
#endif

typedef struct
{
    ull rows, cols;
    T** data;
} matrix;

matrix* alloc(const ull rows, const ull cols)
{
    T** data = (T**) malloc(sizeof(T*) * rows);
    for (ull i = 0; i < rows; ++i) { data[i] = (T*) malloc(sizeof(T) * cols); }

    matrix* A = (matrix*) malloc(sizeof(matrix));
    A->cols = cols;
    A->rows = rows;
    A->data = data;

    return A;
}

#define A(i, j) A->data[i][j]
#define B(i, j) B->data[i][j]
#define C(i, j) C->data[i][j]
#define vA(i)   vA->data[0][i]
#define vB(i)   vB->data[0][i]

void fill(const matrix* A)
{
    srand(time(NULL));

    iterate(ull, i, A->rows) {
        iterate(ull, j, A->cols) {
            A(i, j) = rand() % RMAX;
        }
    }
}

void reset(const matrix* A)
{
    iterate(ull, i, A->rows) {
        iterate(ull, j, A->cols) { A(i, j) = (T) 0; }
    }
}

void write(const matrix* A, const char* filename)
{
    FILE* file = fopen(filename, "w");
    if (!file) { perror("Failed while opening file"); }

    iterate(ull, i, A->rows) {
        iterate(ull, j, A->cols) {
            fprintf(file, TOKEN, A(i, j));
            fprintf(file, "\t");
        }
        fprintf(file, "\n");
    }

    fclose(file);
}

/**
 * Cells in a row should be separated with TAB
 * Rows should be separated with NEWLINE
 */

void read(const matrix* A, const char* filename)
{
    FILE* file = fopen(filename, "r");
    if (!file) { perror("Failed while opening file"); }

    iterate(ull, i, A->rows) {
        iterate(ull, j, A->cols) {
            if (!fscanf(file, TOKEN, &A(i, j))) {
                perror("Failed reading matrix from file");
                exit(1);
            }
        }
    }

    fclose(file);
}

void print(const matrix* A)
{
    iterate(ull, i, A->rows) {
        iterate(ull, j, A->cols) {
            printf(TOKEN, A(i, j));
            printf("\t");
        }
        printf("\n");
    }
}

void dealloc(matrix* A)
{
    iterate(ull, i, A->rows) { free(A->data[i]); }
    free(A->data);
    free(A);
}

#endif // MATRIX_H
