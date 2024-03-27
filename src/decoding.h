#ifndef DECODING_H
#define DECODING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "display_variables.h"
#include "defs.h"

// Function to check if it is a valid codeword
int check_codeword(pchk H, int codeword[CODEWORD_LEN]);

// Function to compute a BSC a priori probabilities
float* bsc_a_priori_probabilities(int codeword[CODEWORD_LEN]);

// Function to return a priori probabilities
void a_priori_probabilities(int mode, int codeword[CODEWORD_LEN], float** probabilities);

// Function to compute the product of the other probabilities
float product(float** M, int m, int n);

// Function to decode the message
void decode(pchk H, int* recv_codeword, int* codeword_decoded);

// Function to check all possible codewords
//THIS FUNCTION DOESNT EXIST!
void check_possible_codewords(pchk H);

#endif