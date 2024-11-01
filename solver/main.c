#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "solver.h"

int BUFFER_SIZE = 1;

char **read_file(char **filename)
{
    FILE *file = fopen(filename, "rb");

    // File Size Mesurement See:
    // https://stackoverflow.com/questions/238603/how-can-i-get-a-files-size-in-c
    fseek(file, 0, SEEK_END); // seek to end of file
    int nChar = ftell(file);  // get current file pointer
    fseek(file, 0, SEEK_SET); // seek back to beginning of file

    printf("File Size : %i\n", nChar);

    int lineLen = 0;

    int c;

    do
    {
        c = fgetc(file);
        ++lineLen;
    } while (!(c == EOF || c == '\n'));
    printf("String Size : %i\n", lineLen);

    const char *contents[nChar % lineLen];
    char buffer[lineLen];
    fseek(file, 0, SEEK_SET);
    int counter = 0;
    int curLine = 0;
    do
    {
        c = fgetc(file);

        buffer[counter % lineLen] = c;
        if (counter % lineLen == lineLen - 1)
        {
            // We have arrived at the end of a line. We add 0 to complete the
            // str.
            //  We create a space for this line and then copy the line into the
            //  newly allocated space.
            buffer[lineLen - 1] = 0;
            contents[curLine] = malloc(lineLen);
            strcpy(contents[curLine], buffer);
            curLine++;
        }

        counter++;
    } while (!(c == EOF));

    fclose(file);
    return contents;
}

int main(int argc, char **argv)
{
    const int rows = 14;
    const int columns = 12;

    struct word_coord* arr = malloc(4 * sizeof(int));
    char* mal = malloc(14 * 12 * sizeof(char));

    char matrix[14 * 12] = { 
                        'M', 'S', 'W', 'A', 'T', 'E', 'R', 'M', 'E', 'L', 'O', 'N',
                        'Y', 'T', 'B', 'N', 'E', 'P', 'E', 'W', 'R', 'M', 'A', 'E',
                        'R', 'R', 'L', 'W', 'P', 'A', 'P', 'A', 'Y', 'A', 'N', 'A',
                        'R', 'A', 'N', 'L', 'E', 'M', 'O', 'N', 'A', 'N', 'E', 'P',
                        'E', 'W', 'L', 'E', 'A', 'P', 'R', 'I', 'A', 'B', 'P', 'R',
                        'B', 'B', 'I', 'L', 'B', 'B', 'W', 'B', 'R', 'L', 'A', 'Y',
                        'K', 'E', 'M', 'P', 'M', 'A', 'W', 'L', 'R', 'A', 'R', 'B',
                        'C', 'R', 'E', 'P', 'R', 'N', 'R', 'E', 'R', 'R', 'G', 'R',
                        'A', 'R', 'Y', 'A', 'Y', 'A', 'O', 'A', 'N', 'L', 'A', 'M',
                        'L', 'Y', 'Y', 'A', 'R', 'N', 'E', 'R', 'K', 'I', 'W', 'I',
                        'B', 'E', 'B', 'A', 'A', 'A', 'N', 'A', 'A', 'P', 'R', 'T',
                        'Y', 'R', 'R', 'E', 'B', 'P', 'S', 'A', 'R', 'N', 'N', 'W',
                        'Y', 'R', 'R', 'E', 'B', 'E', 'U', 'L', 'B', 'L', 'G', 'I',
                        'T', 'Y', 'P', 'A', 'T', 'E', 'A', 'E', 'P', 'A', 'C', 'E',
                            };

    for (int x = 0; x < rows; x++)
    {   
        mal[x * columns + 0] = matrix[x * columns + 0]; 
        // printf("%c",mal[x * columns + 0]);
        for (int y = 1; y < columns; y++)
        {
            mal[x * columns + y] = matrix[x * columns + y];
            // printf(" %c",mal[x * columns + y]);
        }
        // printf("\n");
    }

    if (find_word_matrix(mal, rows, columns, argv[1], atoi(argv[2]), arr) == 1){
        printf("\n (1) The word has been found at index [%i][%i] to [%i][%i]\n", arr->xstart,arr->ystart,arr->xend,arr->yend);
        free(mal);
        free(arr);
    }
    // else{
    //    printf("\n (0) The word is not in the grid\n");
    // }
   
    // if (argc < 2)
    // {
    //    err(1, "Solver usage : ./solver <filename>");
    // }
    // const char **gridLines = read_file(argv[1]);
    //return 0;
}