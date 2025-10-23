#ifdef PARALLEL

#include <macros.h>
#include <simple_buffers.h>
#include <master.h>
#include <slave.h>
#include <inttypes.h>
#include <stdio.h>
#include <mpi.h>

int main(int argc, char ** argv){
    u32 N;
    if(argc < 2) {
        fprintf(stderr, "Please, provide an integer between 4 and 4294967295 to the algorithm\n");
        return -1;
    }
    if(sscanf(argv[1], "%" SCNu32, &N) != 1) {
        fprintf(stderr, "Invalid input provided, please type an integer between 4 and 4294967295 to the algorithm\n");
        return -1;
    }
    int rank, num_procs;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    int const num_slaves = num_procs - 1;
    u32 const num_odds = (N - 1) / 2;
    u64 const slice_size = num_odds / num_slaves;
    if(num_procs < 3){
        fprintf(stderr, "Process count lower than 3.\nMaster-Slave strategy won't work in this case, use more processors\n");
        MPI_Finalize();
        return -1;
    }
    if(rank == 0){
        printf("Running program in parallel mode (%d procecess).\n", num_procs);
        master_init(N, num_slaves);
        master_loop();
        #ifdef PRINT_RESULT
        printf("Primes found: [%" PRIu32, master_primes_found.m_buffer[0]);
        for(u32 i = 1; i < master_primes_found_len; i += 1)
            printf(", %" PRIu32, master_primes_found.m_buffer[i]);
        printf("]\n");
        #endif // PRINT_RESULT
        master_finish();
        MPI_Finalize();
        return 0;
    }
    slave_init(rank, slice_size);
    slave_loop();
    slave_finish();
    MPI_Finalize();
    return 0;
}

#endif // PARALLEL