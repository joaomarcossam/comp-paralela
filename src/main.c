#ifndef PARALLEL
#include <macros.h>
#include <sequential_Erastotenes.h>
#include <limits.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdio.h>

int main(int argc, char **argv) {
  if(argc < 2) {
    fprintf(stderr, "Please, provide an integer between 4 and 4294967295 to the algorithm\n");
    return -1;
  }
  u32 N;
  if(sscanf(argv[1], "%" SCNu32, &N) != 1) {
    fprintf(stderr, "Invalid input provided, please type an integer between 4 and 4294967295 to the algorithm\n");
    return -1;
  }
  u32Buffer primes = sequential_primes_until(N);
  printf("Running program in sequential mode.\n");
  #ifdef PRINT_RESULT
  printf("Primes found: [%" PRIu32, primes.m_buffer[0]);
  for(u32 i = 1; i < primes.m_buffer_size; i += 1)
    printf(", %" PRIu32, primes.m_buffer[i]);
  printf("]\n");
  #endif
  u32_buffer_free(&primes);
  return 0;
}

#endif // PARALLEL