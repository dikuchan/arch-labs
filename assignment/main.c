#include <stdio.h>
#include <string.h>

#include "minmax.h"
#include "multiplication.h"
#include "vector.h"
#include "pattern.h"

const char* info =
    "Usage: assignment [task]\n";

int main(int argc, char** argv)
{
    if (argc == 1) {
#ifdef INLINE
        ull r  = 102, c  = 102,
            ar = 102, ac = 102,
            br = 102, bc = 102;
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

        printf(
            "\n(1)\n\tRows: %llu\n\tCols: %llu\n\tThreads: %lu\n\n",
            r,
            c,
            threads);
        printf("\tMinmax time: %llu μs\n", minmax(100, 100, 16));

        /**
         * Multiplication (Task 2 & 4)
         */

        printf("\n(2)\n");
        printf("\tA:\n\t\tRows: %llu\n\t\tCols: %llu\n", ar, ac);
        printf("\tB:\n\t\tRows: %llu\n\t\tCols: %llu\n", br, bc);
        printf("\tThreads: %lu\n\n", threads);
        printf(
            "\tStandard multiplication time: %llu μs\n",
            mstandard(ar, ac, br, bc, threads));
        printf(
            "\tBlocks multiplication time: %llu μs\n",
            mblocks(ar, ac, br, bc, threads));

        /**
         * Vector multiplication (Task 3)
         */

        printf(
            "\n(3)\n\tRows: %llu\n\tCols: %llu\n\tThreads: %lu\n\n",
            r,
            c,
            threads);
        printf("\tSplit by rows time: %llu μs\n", vrows(r, c, threads));
        printf("\tSplit by cols time: %llu μs\n", vcols(r, c, threads));
        printf("\tSplit by blocks time: %llu μs\n", vblocks(r, c, threads));

        /**
         * Pattern matching (Task 5)
         */

        printf("\n(4)\n");
        printf("\tPattern matching time: %llu μs\n", pattern(filename, mask));
    }
    else if (argc == 2) {
        if (!strcmp("-i", argv[1])) {
            printf("%s\n", info);
        }
    }

    return 0;
}
