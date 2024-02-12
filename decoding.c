#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_ITERATIONS 5

#define CODEWORD_LEN 6 // N
#define MESSAGE_LEN 3  // K
#define NUM_CHECKS 4   // M

#define BSC_MODE 0
#define AWGN_MODE 1

#define CURR_MODE BSC_MODE

#define BSC_ERROR_RATE 0.2

// Function to print a vector int
void print_vector_int(int vector[], int len)
{
    printf("[ ");
    for (int i = 0; i < len; i++)
        printf("%d ", vector[i]);
    printf("]\n");
}

// Function to print a vector float
void print_vector_float(float vector[], int len)
{
    printf("[ ");
    for (int i = 0; i < len; i++)
        printf("%f ", vector[i]);
    printf("]\n");
}

// Function to print a matrix int
void print_matrix_int(int** matrix, int rows, int cols)
{
    for (int i = 0; i < rows; i++)
    {
        printf("[ ");
        for (int j = 0; j < cols; j++)
            printf("%d ", matrix[i][j]);
        printf("]\n");
    }
}

// Function to print a matrix float
void print_matrix_float(float** matrix, int rows, int cols)
{
    for (int i = 0; i < rows; i++)
    {
        printf("[ ");
        for (int j = 0; j < cols; j++)
            printf("%f ", matrix[i][j]);
        printf("]\n");
    }
}


// Function to check if it is a valid codeword
int check_codeword(int H[NUM_CHECKS][CODEWORD_LEN], int codeword[CODEWORD_LEN])
{
    int check = 0;

    for (int m = 0; m < NUM_CHECKS; m++)
    {
        for(int n = 0; n < CODEWORD_LEN; n++)
        {
            if (codeword[n] == 1)
            {
                check ^= H[m][n];
            }
        }
        if (check == 1)
        {
            return 0;
        }
    }

    return 1;

}

// Function to compute a BSC a priori probabilities
float* bsc_a_priori_probabilities(int codeword[CODEWORD_LEN])
{
    float *probabilities = (float*)malloc(CODEWORD_LEN * sizeof(float));
    for (int i = 0; i < CODEWORD_LEN; i++)
    {
        if (codeword[i] == 0)
        {
            probabilities[i] = log((1 - BSC_ERROR_RATE)/BSC_ERROR_RATE);
        }
        else
        {
            probabilities[i] = log(BSC_ERROR_RATE/(1 - BSC_ERROR_RATE));
        }
    }
    return probabilities;
}

// Function to return a priori probabilities
void a_priori_probabilities(int mode, int codeword[CODEWORD_LEN], float** probabilities)
{
    switch (mode)
    {
    case BSC_MODE:
        *probabilities = bsc_a_priori_probabilities(codeword);
        break;
    default:
        break;
    }
}

float product(float** M, int m, int n)
{
    float product = 1;
    for (int i = 0; i < CODEWORD_LEN; i++)
    {
        if (i != n && M[m][i] != 0)
        {
            product *= tanh(M[m][i]/2);
        }
    }
    return product;
}


// Function to decode the message
int* decode(int H[NUM_CHECKS][CODEWORD_LEN], int codeword[CODEWORD_LEN])
{
    float *probabilities = (float*)malloc(CODEWORD_LEN * sizeof(float));

    float **M;
    float **extrinsic_probabilities;
    float product_value = 0;
    float L[CODEWORD_LEN];
    int *codeword_decoded = (int*)malloc(CODEWORD_LEN * sizeof(int));
    int try_n = 0;

    M = (float**)malloc(NUM_CHECKS * sizeof(float*));
    extrinsic_probabilities = (float**)malloc(NUM_CHECKS * sizeof(float*));
    for (int i = 0; i < NUM_CHECKS; i++)
    {
        extrinsic_probabilities[i] = (float*)malloc(CODEWORD_LEN * sizeof(float));
        M[i] = (float*)malloc(CODEWORD_LEN * sizeof(float));
    }

    printf("---------------------INITIALIZATIONS----------------------\n");
    
    printf("Codeword: ");
    print_vector_int(codeword, CODEWORD_LEN);
    printf("\n");
    

    //Compute LLR a priori probabilities (r)
    a_priori_probabilities(CURR_MODE, codeword, &probabilities);

    // AWGN TEST PRESENT ON PAGE 36
    // float test_awgn[CODEWORD_LEN] = {-0.5, 2.5, -4.0, 5.0, -3.5, 2.5};

    // probabilities = (float *)malloc(CODEWORD_LEN * sizeof(float));

    // for (int i = 0; i < CODEWORD_LEN; i++)
    // {
    //     probabilities[i] = test_awgn[i];
    // }
    
    printf("Probabilities: ");
    print_vector_float(probabilities, CODEWORD_LEN);
    
    // Initialize matrix M
    for (int m = 0; m < NUM_CHECKS; m++)
    {
        for (int n = 0; n < CODEWORD_LEN; n++)
        {
            if (H[m][n] == 0)
                M[m][n] = 0;
            else
                M[m][n] = probabilities[n];
        }
    }

    printf("Matrix M: \n");
    print_matrix_float(M, NUM_CHECKS, CODEWORD_LEN);

    while (try_n < MAX_ITERATIONS)
    {
        try_n++;
        printf("---------------------ITERATION %d----------------------\n", try_n);
        // Compute extrinsic probabilities matrix (E)
        for (int i = 0; i < NUM_CHECKS; i++)
        {
            for (int j = 0; j < CODEWORD_LEN; j++)
            {
                extrinsic_probabilities[i][j] = 0;
            }
        }

        for (int m = 0; m < NUM_CHECKS; m++)
        {
            for (int n = 0; n < CODEWORD_LEN; n++)
            {
                if (H[m][n] == 0)
                {
                    extrinsic_probabilities[m][n] = 0;
                } else {
                    product_value = product(M, m, n);
                    extrinsic_probabilities[m][n] = log((1+product_value)/(1-product_value));
                }
            }
        }

        printf("Extrinsic probabilities: \n");
        print_matrix_float(extrinsic_probabilities, NUM_CHECKS, CODEWORD_LEN);

        // Test
        for(int n = 0; n < CODEWORD_LEN; n++)
        {
            L[n] = probabilities[n];
            for(int m = 0; m < NUM_CHECKS; m++)
            {
                L[n] += extrinsic_probabilities[m][n];
            }
            codeword_decoded[n] = (L[n] < 0) ? 1 : 0;
        }
        printf("Final L: ");
        print_vector_float(L, CODEWORD_LEN);
        // Check if it is a valid codeword
        if (check_codeword(H, codeword_decoded) == 1)
        {
            printf("Completed after %d iterations\n", try_n);
            return codeword_decoded;
        }

        // Update matrix M
        for (int n = 0; n < CODEWORD_LEN; n++)
        {
            for (int m = 0; m < NUM_CHECKS; m++)
            {
                if (H[m][n] == 0)
                {
                    M[m][n] = 0;
                }
                else
                {
                    M[m][n] = L[n] - extrinsic_probabilities[m][n];
                }
                
            }
        }
    }

    printf("Not completed after %d iterations\n", try_n);

    return NULL;
    
}

void possible_codeword_list(int ***list)
{
    *list = (int**)malloc(64 * sizeof(int*));
    for (int i = 0; i < 64; i++)
    {
        (*list)[i] = (int*)malloc(CODEWORD_LEN * sizeof(int));
    }

    for (int i = 0; i < 64; i++)
    {
        for (int j = 0; j < CODEWORD_LEN; j++)
        {
            (*list)[i][CODEWORD_LEN - 1 - j] = (i >> j) & 1;
        }
    }
}

// Function to check all possible codewords
void check_possible_codewords(int H[NUM_CHECKS][CODEWORD_LEN])
{
    int *codeword_decoded;
    int **possible_codeword_l;
    possible_codeword_list(&possible_codeword_l);
    
    codeword_decoded = (int*)malloc(CODEWORD_LEN * sizeof(int));
    int check = 0;
    for (int i = 0; i < 64; i++)
    {
        check = check_codeword(H, possible_codeword_l[i]);
            printf("[ ");
            for (int j = 0; j < CODEWORD_LEN; j++)
                printf("%d ", possible_codeword_l[i][j]);
            printf("] -> ");
        if (check == 1)
        {
            printf("Valid codeword\n");
        }
        else
        { 
            codeword_decoded = decode(H, possible_codeword_l[i]);
            if (codeword_decoded == NULL)
            {
                printf("Not a valid codeword\n");
                continue;
            }
            else
            {
                print_vector_int(codeword_decoded, CODEWORD_LEN);   
            }
        }
    }
}

int main()
{
    int H[NUM_CHECKS][CODEWORD_LEN] = {{1, 1, 0, 1, 0, 0},
                                       {0, 1, 1, 0, 1, 0},
                                       {1, 0, 0, 0, 1, 1},
                                       {0, 0, 1, 1, 0, 1}};

    int codeword[CODEWORD_LEN] = {1, 0, 1, 0, 1, 1};

    int *codeword_decoded = decode(H, codeword);

    if(codeword_decoded == NULL)
    {
        printf("Not a valid codeword\n");
        return 0;
    }
    print_vector_int(codeword_decoded, CODEWORD_LEN);

    //check_possible_codewords(H);
    return 0;
}