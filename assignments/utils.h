#ifndef UTILS_H
#define UTILS_H

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

T readnum()
{
    T number;
    char input[20], *pointer;

    fgets(input, 20, stdin);
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

#define ELAPSED (double)(clock() - start) / CLOCKS_PER_SEC

#endif // UTILS_H
