#ifndef MASTER_H
#define MASTER_H

#include <macros.h>
#include <simple_buffers.h>
#include <sys/types.h>
#include <stdint.h>
#include <stdbool.h>
#include <mpi.h>

extern u32Buffer master_primes_found;
extern u32 master_primes_found_len;

bool master_init(u32 const N, int const number_of_slaves);
void master_loop();
void master_finish();

#endif // MASTER_H