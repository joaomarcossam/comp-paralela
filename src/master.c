#include <master.h>

#include <macros.h>
#include <simple_buffers.h>
#include <stdbool.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <mpi.h>

#define RECOVER_FROM_SLAVE_SYNC(BUFFER) MPI_Recv(BUFFER, 1, MPI_UINT32_T, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE)
#define MESSAGE_SLAVE_SYNC(BUFFER, SLAVE_INDEX) MPI_Send(BUFFER, 1, MPI_UINT32_T, SLAVE_INDEX, 0, MPI_COMM_WORLD);
#define SLAVE_FINISHED_PRIME_SEARCH 0
#define SLAVE_BEGIN_PRIME_SEARCH 0

static MPI_Request * bcast_requests = null;
static u32 * bcast_prime_buffer = null;
static int slave_searching_primes = 0;
static int num_slaves;
u32Buffer master_primes_found = NULL_U32BUFFER;
u32 master_primes_found_len = 0;

static void master_bcast_prime(u32 const prime, int const bcast_from) {
  if(master_primes_found_len + 1 >= master_primes_found.m_buffer_size) {
    u32Buffer new_primes_buffer = u32_buffer_init((master_primes_found_len + 1) * 2);
    if(IS_NULL_BUFFER(new_primes_buffer)) {
      fprintf(stderr, "Memory error getting more memory to alloc primes");
      return;
    }
    for(u32 i = 0; i < master_primes_found_len; i += 1)
      new_primes_buffer.m_buffer[i] = master_primes_found.m_buffer[i];
    u32_buffer_free(&master_primes_found);
    master_primes_found = new_primes_buffer;
  }
  master_primes_found.m_buffer[master_primes_found_len++] = prime;
  int const slaves_in_cast = num_slaves - bcast_from + 1;
  u32 * const prime_buffer = bcast_prime_buffer + bcast_from - 1;
  MPI_Request * const requests = bcast_requests + bcast_from - 1;
  for (int i = 0; i < slaves_in_cast; i += 1)
    prime_buffer[i] = prime;
  for(int slave_index = 0; slave_index < slaves_in_cast; slave_index += 1)
    MPI_Isend(
        prime_buffer + slave_index,
        1,
        MPI_UINT32_T,
        slave_index + bcast_from,
        0,
        MPI_COMM_WORLD,
        requests + slave_index);
  MPI_Waitall(slaves_in_cast, requests, MPI_STATUSES_IGNORE);
}

static bool master_update_slave_search_for_prime() {
  if(slave_searching_primes >= num_slaves)
    return false;
  slave_searching_primes += 1;
  u32 message = SLAVE_BEGIN_PRIME_SEARCH;
  MESSAGE_SLAVE_SYNC(&message, slave_searching_primes);
  return true;
}

bool master_init(u32 const N, int const ns) {
  bcast_requests = malloc(sizeof(MPI_Request) * ns);
  bcast_prime_buffer = malloc(32 * ns);
  master_primes_found = u32_buffer_init((size_t)(1.3 * (N/log((long double)N))));
  if((not bcast_requests) or (not bcast_prime_buffer) or IS_NULL_BUFFER(master_primes_found)) {
    fprintf(stderr, "Error getting memory for master internal vectors\n");
    free(bcast_requests);
    free(bcast_prime_buffer);
    u32_buffer_free(&master_primes_found);
    LOG("Failed to initialise the master");
    return false;
  }
  for(int i = 0; i < ns; i += 1)
    bcast_requests[i] = MPI_REQUEST_NULL;
  num_slaves = ns;
  master_update_slave_search_for_prime();
  master_primes_found.m_buffer[0] = 2;
  master_primes_found_len = 1;
  LOG("Master initialised");
  return true;
}

void master_loop() {
  LOG("Begining master loop");
  bool loop_continue = true;
  u32 recieved_prime;
  while(loop_continue) {
    RECOVER_FROM_SLAVE_SYNC(&recieved_prime);
    LOG("Master loop iteration, recieved message from slave: %" PRIu32 "\n", recieved_prime);
    if(recieved_prime == SLAVE_FINISHED_PRIME_SEARCH) {
      loop_continue = master_update_slave_search_for_prime();
      LOG("The message to change slave searching prime was parsed, will the loop end? %s", loop_continue ? "no" : "yes");
      continue;
    }
    else {
      LOG("About to bcast the prime number: %d to all the slaves starting from: %d", recieved_prime, slave_searching_primes + 1);
      master_bcast_prime(recieved_prime, slave_searching_primes + 1);
    }
  }
}

void master_finish() {
  free(bcast_requests);
  free(bcast_prime_buffer);
  u32_buffer_free(&master_primes_found);
  LOG("Master finished");
}

#undef RECOVER_FROM_SLAVE_SYNC
#undef MESSAGE_SLAVE_SYNC
#undef SLAVE_FINISHED_PRIME_SEARCH
#undef SLAVE_BEGIN_PRIME_SEARCH