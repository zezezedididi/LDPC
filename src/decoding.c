#include "decoding.h"

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

// Function to return a priori probabilities based on the chosen mode
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

// Function to compute the product of the other probabilities
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
void decode(int H[NUM_CHECKS][CODEWORD_LEN], int *recv_codeword, int *codeword_decoded)
{
    float *probabilities;
    float **M;
    float **extrinsic_probabilities;
    float product_value = 0;
    float L[CODEWORD_LEN];
    int try_n = 0;

    if (recv_codeword == NULL)
    {
#ifdef DEBUG
        printf("Not a valid codeword\n");
#endif
        return ;
    }
    if(check_codeword(H, recv_codeword) == 1){
        memcpy(codeword_decoded, recv_codeword, CODEWORD_LEN * sizeof(int));
        return ;
    }

    // Initialize variables

    M = (float**)malloc(NUM_CHECKS * sizeof(float*));
    extrinsic_probabilities = (float**)malloc(NUM_CHECKS * sizeof(float*));
    for (int i = 0; i < NUM_CHECKS; i++)
    {
        extrinsic_probabilities[i] = (float*)malloc(CODEWORD_LEN * sizeof(float));
        M[i] = (float*)malloc(CODEWORD_LEN * sizeof(float));
    }
#ifdef DEBUG
    printf("---------------------INITIALIZATIONS----------------------\n");
    
    printf("Codeword: ");
    print_vector_int(recv_codeword, CODEWORD_LEN);
    printf("\n");
#endif

    //Compute LLR a priori probabilities (r)
    a_priori_probabilities(CURR_MODE, recv_codeword, &probabilities);

    // AWGN TEST PRESENT ON PAGE 36
    // float test_awgn[CODEWORD_LEN] = {-0.5, 2.5, -4.0, 5.0, -3.5, 2.5};

    // probabilities = (float *)malloc(CODEWORD_LEN * sizeof(float));

    // for (int i = 0; i < CODEWORD_LEN; i++)
    // {
    //     probabilities[i] = test_awgn[i];
    // }

#ifdef DEBUG
    printf("Probabilities: ");
    print_vector_float(probabilities, CODEWORD_LEN);
#endif

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
#ifdef DEBUG
    printf("Matrix M: \n");
    print_matrix_float(M, NUM_CHECKS, CODEWORD_LEN);
#endif
    while (try_n < MAX_ITERATIONS)
    {
        try_n++;

#ifdef DEBUG
        printf("---------------------ITERATION %d----------------------\n", try_n);
#endif

        // Compute extrinsic probabilities matrix (E)
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
#ifdef DEBUG
        printf("Extrinsic probabilities: \n");
        print_matrix_float(extrinsic_probabilities, NUM_CHECKS, CODEWORD_LEN);
#endif
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
#ifdef DEBUG
        printf("Final L: ");
        print_vector_float(L, CODEWORD_LEN);
#endif
        // Check if it is a valid codeword
        if (check_codeword(H, codeword_decoded) == 1)
        {
#ifdef DEBUG
            printf("Completed after %d iterations\n", try_n);
#endif
            free(probabilities);
            for (int i = 0; i < NUM_CHECKS; i++)
            {
                free(M[i]);
                free(extrinsic_probabilities[i]);
            }
            free(M);
            free(extrinsic_probabilities);
            return ;
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
#ifdef DEBUG
    printf("Not completed after %d iterations\n", try_n);  
#endif
    codeword_decoded = NULL;
    free(probabilities);
    for (int i = 0; i < NUM_CHECKS; i++)
    {
        free(M[i]);
        free(extrinsic_probabilities[i]);
    }
    free(M);
    free(extrinsic_probabilities);
    return ;
}