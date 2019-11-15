#ifndef MATRIX_H
#define MATRIX_H

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

/**
 * Parameters are defined in compile time.
 * Use either `-D` flag or CMake declaration.
 *
 * #define INLINE : If defined, does not require user input.
 * #define WRITE  : If defined, writes matrix to file.
 * #define RMAX   : Random generator range.
 *
 * #define FLOATING : Use double as base type.
 * #define UNSIGNED : Use unsigned integer as base type.
 * DEFAULT          : Signed long long integer.
 */

#define RMAX 40

#if defined(FLOATING)
    typedef double             T;
    #define TOKEN            "%f"
#elif defined(UNSIGNED)
    typedef unsigned long long T;
    #define TOKEN          "%ull"
#else
    typedef long long          T;
    #define TOKEN          "%lli"
    #define MINIMUM      LONG_MIN
    #define MAXIMUM      LONG_MAX
#endif

typedef unsigned long long ull;

struct matrix {
    ull rows, cols;
    T** data;
};

typedef struct matrix matrix;

matrix* alloc(const ull rows, const ull cols) {
    T** data = (T**)malloc(sizeof(T*) * rows);
    for (ull i = 0; i < rows; ++i)
        { data[i] = (T*) malloc(sizeof(T) * cols); }

    matrix* A = (matrix*)malloc(sizeof(matrix));
    A->cols = cols;
    A->rows = rows;
    A->data = data;

    return A;
}

#define readnum(result)                      \
        fgets(input, 20, stdin);             \
        result = strtol(input, &pointer, 10);

#define A(i, j) A->data[i][j]
#define B(i, j) B->data[i][j]
#define C(i, j) C->data[i][j]

#define iterate(type, i, range) for (type i = 0; i < range; ++i)

void fill(matrix* A) {
    srand(time(NULL));

    iterate(ull, i, A->rows) {
        iterate(ull, j, A->cols) {
#if defined(INLINE)
            A(i,j) = rand() % RMAX;
#else
            char input[20], *pointer;
            T result;
            readnum(result);
#endif
        }
    }
}

void write(const matrix* A, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) { perror("Failed while opening file."); }

    iterate(ull, i, A->rows) {
        iterate(ull, j, A->cols) {
            fprintf(file, TOKEN, A(i, j));
            fprintf(file, "\t");
        }
        fprintf(file, "\n");
    }

    fclose(file);
}

void print(const matrix* A) {
    iterate(ull, i, A->rows) {
        iterate(ull, j, A->cols) {
            printf(TOKEN, A(i, j));
            printf("\t");
        }
        printf("\n");
    }
}

void dealloc(matrix* A) {
    iterate(ull, i, A->rows)
        { free(A->data[i]); }
    free(A->data);
    free(A);
}

#endif // MATRIX_H
