#pragma once
#include <stdint.h>
#include <stdlib.h>

/**
 * Computes the sigmoid function.
 */
void sigmoid(size_t size, const float input[], float output[]);

/**
 * Computes the derivative of the sigmoid function.
 */
void sigmoid_prime(size_t size, const float input[], float output[]);

/**
 * Computes the softmax function.
 * (exp(x - max(x)))/(sum(exp(x - max(x))))
 */
void softmax(size_t size, const float input[], float output[]);

/**
 * Generate a normally distributed random number using Box-Muller
 * see https://rh8liuqy.github.io/Box_Muller_Algorithm.html
 */
float randn();
