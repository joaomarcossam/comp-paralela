#ifndef SLAVE_H
#define SLAVE_H
#include <stdlib.h>
#include <stdint.h>
#include <macros.h>

void slave(const int rank, const size_t slice_size);
void mark_prime(const u32 prime, i8Buffer numbers, const u32 from, const size_t first_num);

#endif // SLAVE_H