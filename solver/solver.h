#pragma once
#include <stdio.h>

typedef struct word_coord {
    int xstart; // Starting value of x
    int ystart; // Starting value of y
    int xend; // Ending value of x
    int yend; // Ending value of y
} word_coord;

/**
 * Searches for a specific word in a given grid
 * @param matrix The grid given to the function in which
 * it searches for the word
 * @param rows The number of rows of the grid
 * @param columns The number of columns of the grid
 * @param word The word the function searches for as
 * a string
 * @param word_size The size of the word the function
 * searches for
 * @param arr A pointer to the indexes of the start and
 * of the end of the word if found
 * @return 1 if the word has been found else 0 
 */
int find_word_matrix(char* matrix, int rows, int columns, char* word, int word_size, struct word_coord* arr);


/**
 * @param x The starting row of the functions
 * @param y The starting column of the functions
 */

/**
 * Searches leftwards for the word
 */
int find_word_matrix_l(char* matrix, int rows, int columns, char* word, int word_size, int x, int y, struct word_coord* arr);

/**
 * Searches rightwards for the word
 */
int find_word_matrix_r(char* matrix, int rows, int columns, char* word, int word_size, int x, int y, struct word_coord* arr);

/**
 * Searches upwards for the word
 */
int find_word_matrix_u(char* matrix, int rows, int columns, char* word, int word_size, int x, int y, struct word_coord* arr);

/**
 * Searches downwards for the word
 */
int find_word_matrix_d(char* matrix, int rows, int columns, char* word, int word_size, int x, int y, struct word_coord* arr);

/**
 * Searches the upper-right diagonal
 * for the word
 */
int find_word_matrix_ru(char* matrix, int rows, int columns, char* word, int word_size, int x, int y, struct word_coord* arr);

/**
 * Searches the upper-left diagonal
 * for the word
 */
int find_word_matrix_lu(char* matrix, int rows, int columns, char* word, int word_size, int x, int y, struct word_coord* arr);

/**
 * Searches the lower-right diagonal
 * for the word
 */
int find_word_matrix_rd(char* matrix, int rows, int columns, char* word, int word_size, int x, int y, struct word_coord* arr);

/**
 * Searches the lower-left diagonal
 * for the word
 */
int find_word_matrix_ld(char* matrix, int rows, int columns, char* word, int word_size, int x, int y, struct word_coord* arr);