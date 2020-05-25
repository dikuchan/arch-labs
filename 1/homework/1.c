#include <stdio.h>
#include <string.h>

#include "minmax.h"
#include "multiplication.h"
#include "vector.h"
#include "pattern.h"

const char* info =
    "Usage: assignment [task]";

int main(int argc, char** argv)
{
    if (argc == 1) {
#ifdef INLINE
        ull r  = 512, c  = 512,
            ar = 512, ac = 512,
            br = 512, bc = 512;
        ul threads = 8;

        const char* filename = "sample.txt";
        char* mask = "*lsk?";
#else
        ull r  = readnum("Rows"),   c  = readnum("Cols"),
            ar = readnum("A rows"), ac = readnum("A cols"),
            br = readnum("B rows"), bc = readnum("B cols");
        ul threads = readnum("Threads");

        if (ac != br) {
            perror("Wrong dimensions");
            return 1;
        }

        char filename[128], mask[128];

        printf("Filename: ");
        fgets(filename, 128, stdin);
        printf("Pattern mask: ");
        fgets(mask,     128, stdin);

        printf("\n--------------------------------\n");
#endif

        /**
         * Minmax (Task 1)
         */

        printf("\n(1)\n");

        printf("\tRows: %llu\n\tCols: %llu\n\tThreads: %lu\n\n", r, c, threads);

        printf("\tMinmax time: %llu μs\n", minmax(100, 100, 16));

        /**
         * Multiplication (Task 2 & 4)
         */

        printf("\n(2)\n");

        printf("\tA:\n\t\tRows: %llu\n\t\tCols: %llu\n", ar, ac );
        printf("\tB:\n\t\tRows: %llu\n\t\tCols: %llu\n", br, bc );
        printf("\tThreads: %lu\n\n",                     threads);

        printf("\tStandard multiplication time: %llu μs\n",     mstandard    (ar, ac, br, bc, threads));
        printf("\tBlocks multiplication time: %llu μs\n",       mblocks      (ar, ac, br, bc, threads));
        printf("\tCheckerboard multiplication time: %llu μs\n", mcheckerboard(ar, ac, br, bc, threads));

        /**
         * Vector multiplication (Task 3)
         */

        printf("\n(3)\n");

        printf("\tRows: %llu\n\tCols: %llu\n\tThreads: %lu\n\n", r, c, threads);

        printf("\tSplit by rows time: %llu μs\n",   vrows  (r, c, threads));
        printf("\tSplit by cols time: %llu μs\n",   vcols  (r, c, threads));
        printf("\tSplit by blocks time: %llu μs\n", vblocks(r, c, threads));

        /**
         * Pattern matching (Task 5)
         */

        printf("\n(4)\n");
        printf("\tPattern matching time: %llu μs\n", pattern(filename, mask));
    }
    else if (argc == 2) {
        if (!strcmp("-i", argv[1])) { printf("%s\n", info); }
        else if (!strcmp("-o", argv[1])) {
            FILE* file = fopen("statistics.txt", "a");
            const char* filename = "sample.txt";
            char* mask = "*lsk?";

            if (!file) {
                perror("Opening file error");
                return 1;
            }
            fprintf(file, "\n[%s, %s]\n", __DATE__, __TIME__);
            iterate(ull, s, 4096) { // Size of matrices
                iterate(ul, t, 16) { // Number of threads
                    fprintf(file, "\t[%llu %lu]\n", s, t);
                    fprintf(file, "\t\t(1) [Minmax]       %llu\n", minmax       (s, s, t)       );
                    fprintf(file, "\t\t(2) [Standard]     %llu\n", mstandard    (s, s, s, s, t) );
                    fprintf(file, "\t\t(2) [Blocks]       %llu\n", mblocks      (s, s, s, s, t) );
                    fprintf(file, "\t\t(2) [Checkerboard] %llu\n", mcheckerboard(s, s, s, s, t) );
                    fprintf(file, "\t\t(3) [Rows]         %llu\n", vrows        (s, s, t)       );
                    fprintf(file, "\t\t(3) [Cols]         %llu\n", vcols        (s, s, t)       );
                    fprintf(file, "\t\t(3) [Blocks]       %llu\n", vblocks      (s, s, t)       );
                    fprintf(file, "\t\t(4) [Pattern]      %llu\n", pattern      (filename, mask));
                }
            }
            fclose(file);
        }
    }
    else {
        perror("Parsing error");
        return 1;
    }

    return 0;
}
