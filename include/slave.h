#ifndef SLAVE_H
#define SLAVE_H

#include <macros.h>
#include <simple_buffers.h>
#include <sys/types.h>
#include <stdint.h>
#include <stdbool.h>

bool slave_init(int const rank, u32 const slice_size);
void slave_loop();
void slave_finish();

#endif // SLAVE_H