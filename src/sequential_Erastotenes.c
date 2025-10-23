#include <sequential_Erastotenes.h>

#include <macros.h>
#include <simple_buffers.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <stdio.h>

#define IS_PRIME 1
#define NOT_PRIME 0

static inline u32 num_of(u32 const index) {
  return 2 * index + 3;
}

u32Buffer sequential_primes_until(u32 const N) {
  LOG("Started sequential Erastotenes alg with N = %" PRIu32, N);
  if(N <= 3) {
    fprintf(stderr, "Please, call the algorithm with N bigger than 3");
    LOG("Algorithm called with N < 3");
    return NULL_U32BUFFER;
  }
  i8Buffer marked_odds = i8_buffer_init((N + 1) / 2 - 1);
  if(IS_NULL_BUFFER(marked_odds)) {
    fprintf(stderr, "Failed to alloc memory for the array used to tag prime numbers");
    LOG("Returning null buffer due to memory errors");
    return NULL_U32BUFFER;
  }
  memset(marked_odds.m_buffer, IS_PRIME, marked_odds.m_buffer_size);
  LOG("All values marked as prime with memset");
  u32Buffer primes = u32_buffer_init((N + 1) / 2); 
  if(IS_NULL_BUFFER(primes)) {
    fprintf(stderr, "Failed to alloc memory for the array containing the prime numbers");
    LOG("Returning null buffer due to memory errors");
    i8_buffer_free(&marked_odds);
    return NULL_U32BUFFER;
  }
  u32 prime_next_index = 0;
  primes.m_buffer[prime_next_index++] = 2;
  for(u32 i = 0; i < marked_odds.m_buffer_size; i++) {
    if(marked_odds.m_buffer[i] == NOT_PRIME)
      continue;
    u32 const val = num_of(i);
    LOG("Found a prime (number = %" PRIu32 ", index = %" PRIu32 ")", val, i);
    primes.m_buffer[prime_next_index++] = val;
    for(u32 k = (val * val - 3) / 2; k < marked_odds.m_buffer_size; k += val)
      marked_odds.m_buffer[k] = NOT_PRIME;
  }
  primes.m_buffer_size = prime_next_index;
  i8_buffer_free(&marked_odds);
  return primes;
}

#undef IS_PRIME
#undef NOT_PRIME