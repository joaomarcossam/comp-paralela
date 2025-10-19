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
        bool working_procs[size-1];
        bool still_working = true;

        memset(working_procs, 1, size-1);

        u32 received_prime_task[2];
        MPI_Status status;

        //Master
        while (still_working){
            MPI_Recv(
                received_prime_task, 1, MPI_UINT32_T,
                MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD,
                &status
            );

            if (received_prime_task[0] == 0){
                working_procs[status.MPI_SOURCE-1] = 0;
            }

            const int send_to = status.MPI_SOURCE + 1;
            
            if(send_to < size){
                received_prime_task[1] = (send_to-1) * slice_size +1;
                for (;received_prime_task[1] % received_prime_task[0] != 0; received_prime_task[1]++);

                MPI_Send(received_prime_task, 2, MPI_UINT32_T, send_to, 0, MPI_COMM_WORLD);
                working_procs[status.MPI_SOURCE-1] = 1;
            }

            still_working = false;
            for(int i=0; i<size-1; i++){
                if(working_procs[i] == 1){
                    still_working = true;
                    break;
                }
            }
        }

        MPI_Finalize();
        return 0;
    }

    /* 
        - Criar Tasks nos slaves.
        - Sinalizar saída dos processos.
        - Receber e iniciar as Tasks.
    */

    i8Buffer numbers = i8_buffer_init(slice_size);

    const size_t first_num = (rank-1) * slice_size + 1;
    const size_t last_num = first_num + slice_size - 1;

    LOG("Slice %d: from %d to %d\n", rank, first_num, last_num);

    mark_vector(numbers, first_num, last_num);

    i8_buffer_free(&numbers);
    
    
    MPI_Finalize();


    return 0;
}


#endif // PARALLEL