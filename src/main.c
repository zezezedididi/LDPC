#include <time.h>
#include "encoding.h"
#include "decoding.h"
#include "display_variables.h"
#include "defs.h"
#include "storage.h"

//this is to go in the seperate file
#include <string.h>

void BSC_noise(int *codeword, float p)
{
    srand(time(NULL));
    for (int i = 0; i < CODEWORD_LEN; i++)
    {
        float r = (float)rand() / RAND_MAX;
        printf("r = %f\n", r);
        if (r < p)
        {
            codeword[i] = !codeword[i];
        }
    }
}

int main(int argc, char *argv[])
{
    //check input arguments
    if(argc!=3){
        printf("Incorrect usage!\n Correct usage is: ./ldpc G_filepath H_filepath\n");
        exit(1);
    }

    //get parity check matrices from file
    pchk H,G;
    get_matrix_from_file(&H,argv[1]);
    get_matrix_from_file(&G,argv[1]);

#ifdef DEBUG
    print_parity_check(H);
    printf("\n");
    print_parity_check(G);
#endif

    int message[MESSAGE_LEN] = {0, 0, 0};
    
    int *codeword_encoded = (int*)calloc(CODEWORD_LEN,sizeof(int));

    int *codeword_decoded = (int*)calloc(CODEWORD_LEN,sizeof(int));

    //int test_codeword[CODEWORD_LEN] = {1, 1, 1, 1, 1, 1};

    encode((int *)message, G, codeword_encoded);
    
    print_vector_int(codeword_encoded, CODEWORD_LEN);

    //memcpy(codeword_encoded, test_codeword, CODEWORD_LEN*sizeof(int));

    BSC_noise(codeword_encoded, BSC_ERROR_RATE);

    print_vector_int(codeword_encoded, CODEWORD_LEN);
    
    decode(H, codeword_encoded, codeword_decoded);

    if(codeword_decoded == NULL)
    {
        printf("Not a valid codeword\n");
        return 0;
    }
    print_vector_int(codeword_decoded, CODEWORD_LEN);

    free(codeword_encoded);
    free(codeword_decoded);

    //check_possible_codewords(H);
    return 0;
}