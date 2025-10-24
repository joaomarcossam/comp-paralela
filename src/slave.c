#include <slave.h>

#include <macros.h>
#include <simple_buffers.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <mpi.h>

#define IS_PRIME 1
#define NOT_PRIME 0
#define SHOULD_START_PRIME_SEARCH 0
#define SLAVE_FINISHED_PRIME_SEARCH 0
#define MASTER 0
#define MESSAGE_MASTER_SYNC(BUFFER) MPI_Send(BUFFER, 1, MPI_UINT32_T, MASTER, 0, MPI_COMM_WORLD)
#define MESSAGE_MASTER_ASYNC(BUFFER, REQ_PTR) MPI_Isend(BUFFER, 1, MPI_UINT32_T, MASTER, 0, MPI_COMM_WORLD, REQ_PTR)
#define RECOVER_FROM_MASTER_SYNC(BUFFER) MPI_Recv(BUFFER, 1, MPI_UINT32_T, MASTER, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE)

static i8Buffer slice;
static u32 slice_first_num;

inline static u32 num_of(u32 const index) { return index * 2 + slice_first_num; }
inline static u64 index_of(u64 const number) { return (number - slice_first_num) / 2; }

static void mark_multiples(u32 const prime) {
  u64 start_from_num = MAX((((u64)slice_first_num + prime - 1) / prime) * prime, (u64)prime * prime);
  if(start_from_num % 2 == 0)
    start_from_num += prime;
  LOG("Slave [%" PRIu32 ", %" PRIu32 "] marking multiples of %" PRIu32 " beginning in %" PRIu32, slice_first_num, num_of(slice.m_buffer_size - 1), prime, start_from_num);
  for(u64 i = index_of(start_from_num); i < slice.m_buffer_size; i += prime) {
    LOG("Slave [%" PRIu32 ", %" PRIu32 "] marked index %" PRIu32 " (num = %" PRIu32 ")", slice_first_num, num_of(slice.m_buffer_size - 1), i, num_of(i));
    slice.m_buffer[i] = NOT_PRIME;
  }
} 

bool slave_init(int const rank, u32 const slice_size) {
  slice_first_num = slice_size * (rank - 1) * 2 + 3;
  slice = i8_buffer_init(slice_size);
  if(IS_NULL_BUFFER(slice)) {
    fprintf(stderr, "Error initialising slave's slice occurred in rank: %d", rank);
    LOG("Failed to inialise the slave with rank: %d", rank);
    return false;
  }
  memset(slice.m_buffer, IS_PRIME, slice_size);
  LOG("Slave with rank %d initialised successfully.", rank);
  return true;
}

static void slave_prime_search_loop() {
  LOG("Starting search for primes in slave whose first num is %" PRIu32, slice_first_num);
  MPI_Request prime_announce_req;
  for(u32 i = 0; i < slice.m_buffer_size; i += 1) {
    if(slice.m_buffer[i] == NOT_PRIME)
      continue;
    u32 const val = num_of(i);
    MESSAGE_MASTER_ASYNC(&val, &prime_announce_req);
    mark_multiples(val);
    MPI_Wait(&prime_announce_req, MPI_STATUS_IGNORE);
  }
  u32 message = SLAVE_FINISHED_PRIME_SEARCH;
  MESSAGE_MASTER_SYNC(&message);
  LOG("Finished search for primes in slave whose first num is %" PRIu32, slice_first_num);
}

void slave_loop() {
  u32 from_master_buf;
  while(true) {
    RECOVER_FROM_MASTER_SYNC(&from_master_buf);
    if(from_master_buf == SHOULD_START_PRIME_SEARCH) {
      slave_prime_search_loop();
      break;
    }
    mark_multiples(from_master_buf);
  }
}

void slave_finish() {
  LOG("Finished slave whose first num is: %" PRIu32 "successfully", slice_first_num);
  i8_buffer_free(&slice);
}

#undef IS_PRIME
#undef NOT_PRIME
#undef SHOULD_START_PRIME_SEARCH
#undef SLAVE_FINISHED_PRIME_SEARCH
#undef MASTER
#undef MESSAGE_MASTER_ASYNC
#undef MESSAGE_MASTER_SYNC
#undef RECOVER_FROM_MASTER_SYNC