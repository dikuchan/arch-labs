#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <errno.h>

/**
 * Parameters are defined in compile time.
 * Use either `-D` flag or CMake declaration.
 *
 * #define FLOATING : Use double as base type.
 * #define UNSIGNED : Use unsigned integer as base type.
 * DEFAULT          : Signed long long integer.
 */

#if defined(FLOATING)
    typedef double             T;
    #define TOKEN          "%f\n"
#elif defined(UNSIGNED)
    typedef unsigned long long T;
    #define TOKEN        "%ull\n"
#else
    typedef long long          T;
    #define TOKEN        "%lli\n"
    #define MINIMUM      LONG_MIN
#endif

typedef unsigned long long ull;

#define iterate(type, i, range) for (type i = 0; i < range; ++i)

T readnum(FILE* stream, const char* prompt)
{
    T number;
    char input[20], *pointer;

    fputs(prompt, stream);
    fgets(input, 20, stream);
    fputc('\n', stream);
    number = strtol(input, &pointer, 10);

    if (!number) {
        if (errno == EINVAL) {
            perror("Conversion error\n");
            exit(1);
        }
        else if (errno == ERANGE) {
            perror("Value out of range\n");
            exit(1);
        }
    }

    return number;
}

/**
 * ELAPSED : Convert measured time to human-readable format.
 */

#define ELAPSED 1000000LL * \
                (end.tv_sec - start.tv_sec) + \
                (end.tv_usec - start.tv_usec)

#endif // UTILS_H
