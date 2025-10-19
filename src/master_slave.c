#include <master_slave.h>
#include <math.h>
#include <mpi.h>
#define IS_PRIME 1
#define NOT_PRIME 0

inline static u32 num_of(const size_t index, const size_t first_num) {return index + first_num;}
inline static u32 index_of(const u32 number, const size_t first_num) {return number - first_num;}

bool check_prime(int n){
    const int root = sqrt(n);

    for(int i=2; i<=root; i++){
        if(n%i==0){
            return false;
        }
    }
    return true;
}

void annouce_prime(const u32 prime){
    MPI_Send(&prime, 1, MPI_UINT32_T, 0, 0, MPI_COMM_WORLD);
}

void mark_vector(i8Buffer mark_buffer, const size_t first_num, const size_t last_num){
    LOG("Entered mark_vector with numbers from %d to %d\n", first_num, last_num);

    memset(mark_buffer.m_buffer, IS_PRIME, mark_buffer.m_buffer_size);
    
    size_t i = 0;

    for(; i < mark_buffer.m_buffer_size; i++){
        if(check_prime(num_of(i, first_num))){
            announce_prime(num_of(i, first_num));
            break;
        }
        else{
            mark_buffer.m_buffer[i] = NOT_PRIME;
        }
    }

    for(; i < mark_buffer.m_buffer_size; i++){
        if(mark_buffer.m_buffer[i] == NOT_PRIME){
            continue;
        }
        announce_prime(num_of(i, first_num));

        for(size_t j = i*i; j < mark_buffer.m_buffer_size; j + num_of(i, first_num)){
            mark_buffer.m_buffer[j] = NOT_PRIME;
        }
    }
}

#undef IS_PRIME
#undef NOT_PRIME