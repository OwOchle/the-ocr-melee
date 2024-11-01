#include "solver.h"

int find_word_matrix(char* matrix, int rows, int columns, char* word, int word_size, struct word_coord* arr)
{
    for (int x = 0; x < rows; x++) // Traversing rows
    {
        arr->xstart = x;
        for (int y = 0; y < columns; y++) // Traversing columns
        {   
            arr->ystart = y;
            printf("reading matrix[%i][%i] = %c\n",x,y,matrix[x*columns + y]); // Reading the corresponding letter in the grid
            fflush(stdout);
            int up = 0; // 0 => can't proceed downwards for the diags
            int down = 0; // 0 => can't proceed upwards for the diags

            // Check upwards
            if (x - word_size + 1 >= 0)
            {
                if (find_word_matrix_u(matrix, rows, columns, word, word_size, x, y, arr) == 1){
                    return 1;
                }
                up = 1; // The solver can proceed upwards for the 2 diags
            }    

            // Check downwards
            if (x + word_size - 1 < rows)
            {
                if (find_word_matrix_d(matrix, rows, columns, word, word_size, x, y, arr) == 1){
                    return 1;
                }
                down = 1; // The solver can proceed downwards for the 2 diags
            }

            // Check leftwards
            if (y - word_size + 1 >= 0)
            {
                if (find_word_matrix_l(matrix, rows, columns, word, word_size, x, y, arr) == 1){
                    return 1;
                }
                if (up == 1 && find_word_matrix_lu(matrix, rows, columns, word, word_size, x, y, arr) == 1){ // left-upward diagonal
                    return 1;
                }
                if (down == 1 && find_word_matrix_ld(matrix, rows, columns, word, word_size, x, y, arr) == 1){ // left-downward diagonal
                    return 1;
                }

            }

            // Check rightwards
            if (y + word_size - 1 < columns)
            {
                if (find_word_matrix_r(matrix, rows, columns, word, word_size, x, y, arr) == 1){
                    return 1;
                }
                if (up == 1 && find_word_matrix_ru(matrix, rows, columns, word, word_size, x, y, arr) == 1){ // right-upward diagonal
                    return 1;
                }
                if (down == 1 && find_word_matrix_rd(matrix, rows, columns, word, word_size, x, y, arr) == 1){ // right-downward diagonal
                    return 1;
                }
            }

        }
    }
    return 0;
}

int find_word_matrix_l(char* matrix, int rows, int columns, char* word, int word_size, int x, int y, struct word_coord* arr)
{
    printf("checking left\n");
    int i = y - 1;
    int word_index = 1;
    while (i >= 0 && matrix[x*columns + i]== word[word_index]){
        i--;
        word_index++;
        printf("word index = %i\n", word_index);
    }
    if (word_index == word_size){
        arr->xend = x;
        arr->yend = i + 1;
        return 1;
    }
    else
        return 0; 
}

int find_word_matrix_r(char* matrix, int rows, int columns, char* word, int word_size, int x, int y, struct word_coord* arr)
{
    printf("checking right\n");
    int i = y + 1;
    int word_index = 1;
    while (i < columns && matrix[x*columns + i] == word[word_index]){
        i++;
        word_index++;
        printf("word index = %i\n", word_index);
    }
    if (word_index == word_size){
        arr->xend = x;
        arr->yend = i;
        return 1;
    }
    else
        return 0; 
}

int find_word_matrix_u(char* matrix, int rows, int columns, char* word, int word_size, int x, int y, struct word_coord* arr)
{
    printf("checking up\n");
    int i = x - 1;
    int word_index = 1;
    while (i >= 0 && matrix[i*columns + y] == word[word_index]){
        i--;
        word_index++;
        printf("word index = %i\n", word_index);
    }
    if (word_index == word_size){
        arr->xend = i + 1;
        arr->yend = y;
        return 1;
    }
    else
        return 0;
}

int find_word_matrix_d(char* matrix, int rows, int columns, char* word, int word_size, int x, int y, struct word_coord* arr)
{
    printf("checking below\n");
    int i = x + 1;
    int word_index = 1;
    while (i < rows && matrix[i*columns + y] == word[word_index]){
        i++;
        word_index++;
        printf("word index = %i\n", word_index);
    }
    if (word_index == word_size){
        arr->xend = i;
        arr->yend = y;
        return 1;
    }
    else
        return 0; 
}

int find_word_matrix_ru(char* matrix, int rows, int columns, char* word, int word_size, int x, int y, struct word_coord* arr)
{
    printf("checking right-up\n");
    int i = x - 1;
    int j = y + 1;
    int word_index = 1;
    while (i >= 0 && j < columns && matrix[i*columns + j] == word[word_index]){
        i--;
        j++;
        word_index++;
    }
    if (word_index == word_size){
        arr->xend = i + 1;
        arr->yend = j;
        return 1;
    }
    else
        return 0; 
}

int find_word_matrix_lu(char* matrix, int rows, int columns, char* word, int word_size, int x, int y, struct word_coord* arr)
{
    printf("checking left-up\n");
    int i = x - 1;
    int j = y - 1;
    int word_index = 1;
    while (i >= 0 && j >= 0 && matrix[i*columns + j] == word[word_index]){
        i--;
        j--;
        word_index++;
    }
    if (word_index == word_size){
        arr->xend = i + 1;
        arr->yend = j + 1;
        return 1;
    }
    else
        return 0; 
}

int find_word_matrix_rd(char* matrix, int rows, int columns, char* word, int word_size, int x, int y, struct word_coord* arr)
{
    printf("checking right-down\n");
    int i = x + 1;
    int j = y + 1;
    int word_index = 1;
    while (i < rows && j < columns && matrix[i*columns + j] == word[word_index]){
        i++;
        j++;
        word_index++;
    }
    if (word_index == word_size){
        arr->xend = i;
        arr->yend = j;
        return 1;
    }
    else
        return 0; 
}

int find_word_matrix_ld(char* matrix, int rows, int columns, char* word, int word_size, int x, int y, struct word_coord* arr)
{
    printf("checking left-down\n");
    int i = x + 1;
    int j = y - 1;
    int word_index = 1;
    while (i < rows && j >= 0 && matrix[i*columns + j] == word[word_index]){
        i++;
        j--;
        word_index++;
    }
    if (word_index == word_size){
        arr->xend = i;
        arr->yend = j + 1;
        return 1;
    }
    else
        return 0; 
}