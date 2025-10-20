#define PARALLEL
#ifdef PARALLEL
#include <stdio.h>
#include <mpi.h>
#include <macros.h>
#include <inttypes.h>
#include <simple_buffers.h>
#include <master_slave.h>

int main(int argc, char** argv){
    u32 N;
    int rank, size;
    MPI_Init(&argc, &argv);
    
    printf("Main Paralela\n");

    if(argc < 2) {
        fprintf(stderr, "Please, provide an integer between 4 and 4294967295 to the algorithm\n");
        return -1;
    }
    
    if(sscanf(argv[1], "%" SCNu32, &N) != 1) {
        fprintf(stderr, "Invalid input provided, please type an integer between 4 and 4294967295 to the algorithm\n");
        return -1;
    }

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    const size_t slice_size = N/(size-1);

    if(size < 3){
        printf("Process count lower than 3.\nMaster-Slave strategy won't work in this case, use more processors\n");
        MPI_Finalize();
        return -1;
    }

    if(rank == 0){
        master_loop(size, slice_size);
        i8* primes_indexes = malloc(N * sizeof(i8));

        MPI_Gather(NULL, 0, MPI_INT8_T, primes_indexes, N, MPI_INT8_T, 0, MPI_COMM_WORLD);

        free(primes_indexes);
        MPI_Finalize();
        return 0;
    }

    slave(rank, slice_size);
    /* 
        - Criar Tasks nos slaves.
        - Sinalizar saída dos processos.
        - Receber e iniciar as Tasks.
    */

    
    MPI_Finalize();


    return 0;
}


#endif // PARALLEL