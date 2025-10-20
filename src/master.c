#include <master.h>
#include <stdbool.h>
#include <stdint.h>
#include <mpi.h>

#include <macros.h>

void master_loop(const int size, const size_t slice_size){
    bool working_procs[size-1];
    bool still_working = true;

    memset(working_procs, 1, size-1);

    u32 received_prime_task[2];
    MPI_Status status;

    while (working_procs[size-2]){
        MPI_Recv(
            received_prime_task, 1, MPI_UINT32_T,
            MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD,
            &status
        );
        
        if (received_prime_task[0] == 0){
            working_procs[status.MPI_SOURCE-1] = false;
            for(int i = status.MPI_SOURCE-2; i >= 0; i--){
                if(working_procs[i]){
                    received_prime_task[0] = 1;
                    MPI_Send(received_prime_task, 2, MPI_UINT32_T, status.MPI_SOURCE, 0, MPI_COMM_WORLD);
                    working_procs[status.MPI_SOURCE-1] = true;
                    break;
                }
            }
            if(not working_procs[status.MPI_SOURCE-1]){
                MPI_Send(received_prime_task, 2, MPI_UINT32_T, status.MPI_SOURCE, 0, MPI_COMM_WORLD);
            }
            continue;
        }

        const int send_to = status.MPI_SOURCE + 1;
        
        if(send_to >= size) continue;
         
        received_prime_task[1] = (send_to-1) * slice_size +1;
        received_prime_task[1] += received_prime_task[1] % received_prime_task[0];

        MPI_Send(received_prime_task, 2, MPI_UINT32_T, send_to, 0, MPI_COMM_WORLD);
    } 
}