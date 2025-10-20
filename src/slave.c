#include <slave.h>
#include <simple_buffers.h>
#include <string.h>
#include <mpi.h>
#include <stdbool.h>

#define IS_PRIME 1
#define NOT_PRIME 0

inline static u32 num_of(const size_t index, const size_t first_num) {return index + first_num;}
inline static u32 index_of(const u32 number, const size_t first_num) {return number - first_num;}

void slave(const int rank, const size_t slice_size){
    i8Buffer numbers = i8_buffer_init(slice_size);
    memset(numbers.m_buffer, IS_PRIME, slice_size);
    u32 task_buffer[2];
    
    const size_t first_num = (rank-1) * slice_size + 1;
    const size_t last_num = first_num + slice_size - 1;

    LOG("Slice %d: from %d to %d\n", rank, first_num, last_num);

    MPI_Request request;
    MPI_Irecv(task_buffer, 2, MPI_UINT32_T, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &request);
    
    int test_result;

    for(size_t i = 0; i < numbers.m_buffer_size; i++){
        if(numbers.m_buffer[i] == NOT_PRIME) continue;
    
        if((MPI_Test(&request, &test_result, MPI_STATUS_IGNORE), test_result)){
            mark_prime(task_buffer[0], numbers, task_buffer[1], first_num);
            MPI_Irecv(task_buffer, 2, MPI_UINT32_T, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &request);
        }
        const u32 prime_found = num_of(i, first_num);

        mark_prime(prime_found, numbers, prime_found*prime_found, first_num);
    }

    task_buffer[0] = 0;
    MPI_Send(task_buffer, 1, MPI_UINT32_T, 0, 0, MPI_COMM_WORLD);
    MPI_Recv(task_buffer, 2, MPI_UINT32_T, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    
    while(task_buffer[0]){
        //Stop condition
        MPI_Recv(task_buffer, 2, MPI_UINT32_T, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        if(not task_buffer[0]) break;

        mark_prime(task_buffer[0], numbers, task_buffer[1], first_num);
        MPI_Send(task_buffer, 1, MPI_UINT32_T, 0, 0, MPI_COMM_WORLD);
        MPI_Recv(task_buffer, 2, MPI_UINT32_T, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    
    MPI_Gather(numbers.m_buffer, numbers.m_buffer_size, MPI_INT8_T, NULL, 0, MPI_INT8_T, 0, MPI_COMM_WORLD);
    i8_buffer_free(&numbers);
}

void mark_prime(const u32 prime, i8Buffer numbers, const u32 from, const size_t first_num){
    for(size_t i=index_of(from, first_num); i < numbers.m_buffer_size; i += prime){
        numbers.m_buffer[i] = NOT_PRIME;
    }
}

#undef IS_PRIME
#undef NOT_PRIME