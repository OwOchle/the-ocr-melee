#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <string.h>
#include <stdlib.h>

int BUFFER_SIZE = 1;

char ** read_file(char **filename){
    FILE * file = fopen(filename, "rb");

    // File Size Mesurement See: https://stackoverflow.com/questions/238603/how-can-i-get-a-files-size-in-c
    fseek(file, 0, SEEK_END); // seek to end of file
    int nChar = ftell(file); // get current file pointer
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

    const char *contents[nChar%lineLen];
    char buffer[lineLen];
    fseek(file, 0, SEEK_SET);
    int counter = 0;
    int curLine = 0;
    do
    {
        c = fgetc(file);
        
        buffer[counter%lineLen] = c;
        if (counter%lineLen == lineLen-1){
            //We have arrived at the end of a line. We add 0 to complete the str. 
            // We create a space for this line and then copy the line into the newly allocated space.
            buffer[lineLen-1] = 0;
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
    if (argc < 2){
        err(1, "Solver usage : ./solver <filename>");
    }
    const char **gridLines = read_file(argv[1]);
    return 0;
}