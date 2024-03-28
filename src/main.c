#include <time.h>
#include "encoding.h"
#include "decoding.h"
#include "display_variables.h"
#include "defs.h"

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

//this is to go in the seperate file
void **get_matrix_from_file(pchk *matrix,char *filename){
    FILE *f = fopen (filename,"r");
    int i=0;

    //Open file to read
    if(f==NULL){
        printf("couldn't open matrix file %s\n",filename);
        exit(1);
    }    

    //matrix info
    fread(&(matrix->n_row),sizeof(int),1,f);
    fread(&(matrix->n_col),sizeof(int),1,f);
    fread(&(matrix->type),sizeof(int),1,f);

    //A
    matrix->A = (int**)malloc(matrix->n_row*sizeof(int*));
    if(matrix->type ==0){
        //normal
        for(int c=0;c<matrix->n_row;c++){
            matrix->A[c] = (int*)malloc(matrix->n_col*sizeof(int));
            fread(matrix->A[c],sizeof(int),matrix->n_col,f);
        }
    }
    else{
        //sparse
        for(int c=0;c<matrix->n_row;c++){
            fread(&i,sizeof(int),1,f);
            matrix->A[c] = (int*)malloc(i*sizeof(int));
            fread(matrix->A[c],sizeof(int),i,f);
        }
    }
    fclose(f);
    return NULL;
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
    print_matrix_int(H.A,H.n_row,H.n_col);
    printf("\n");
    print_matrix_int(G.A,G.n_row,G.n_col);
#endif

    exit(0);

    int message[MESSAGE_LEN] = {1, 0, 1};
    
    int *codeword_encoded = (int*)malloc(CODEWORD_LEN*sizeof(int));

    int *codeword_decoded = (int*)malloc(CODEWORD_LEN*sizeof(int));

    //int test_codeword[CODEWORD_LEN] = {1, 1, 1, 1, 1, 1};

    encode(message, G, codeword_encoded);
    
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