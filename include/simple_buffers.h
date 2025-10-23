#ifndef SIMPLE_BUFFERS_H
#define SIMPLE_BUFFERS_H

#include <macros.h>
#include <stdint.h>
#include <sys/types.h>

struct __u32_buffer {
  size_t m_buffer_size;
  u32 * m_buffer;
};

struct __i8_buffer {
  size_t m_buffer_size;
  i8 * m_buffer;
};

typedef struct __u32_buffer u32Buffer;
typedef struct __i8_buffer i8Buffer;

u32Buffer u32_buffer_init(size_t const size);
i8Buffer i8_buffer_init(size_t const size);
void u32_buffer_free(u32Buffer * const ptr_to_auto_var);
void i8_buffer_free(i8Buffer * const ptr_to_auto_var);

#define NULL_U32BUFFER (u32Buffer){.m_buffer_size = 0, .m_buffer = null}
#define NULL_I8BUFFER (i8Buffer){.m_buffer_size = 0, .m_buffer = null}
#define IS_NULL_BUFFER(BUFFER) (not BUFFER.m_buffer)

#endif // SIMPLE_BUFFERS_H