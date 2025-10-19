#ifndef MASTER_SLAVE_H
#define MASTER_SLAVE_H
#include <simple_buffers.h>
#include <string.h>
#include <stdbool.h>

struct __task {
    u32 m_prime_found;
    u32 m_next_multiple;
};

typedef struct __task Task;

void mark_vector(i8Buffer mark_buffer, const size_t first_num, const size_t last_num);
int get_next_multiple(i8Buffer maked_buffer);

void announce_prime(const u32 prime);
bool check_prime(int n);


#endif