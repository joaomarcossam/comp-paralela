#include <simple_buffers.h>

#include <macros.h>
#include <stdlib.h>

u32Buffer u32_buffer_init(size_t const size) {
  u32Buffer b;
  if(posix_memalign((void**)&b.m_buffer, 64, size * 4)) {
    LOG("Returning empty buffer due to memory errors");
    return NULL_U32BUFFER;
  }
  b.m_buffer_size = size;
  return b;
}

i8Buffer i8_buffer_init(size_t const size) {
  i8Buffer b;
  if(posix_memalign((void**)&b.m_buffer, 64, size)) {
    LOG("Returning empty buffer due to memory errors");
    return NULL_I8BUFFER;
  }
  b.m_buffer_size = size;
  return b;
}

void u32_buffer_free(u32Buffer * const ptr) {
  LOG("Freed a u32Buffer");
  free(ptr->m_buffer);
  ptr->m_buffer = null;
  ptr->m_buffer_size = 0;
}

void i8_buffer_free(i8Buffer * const ptr) {
  LOG("Freed a i8Buffer");
  free(ptr->m_buffer);
  ptr->m_buffer = null;
  ptr->m_buffer_size = 0;
}