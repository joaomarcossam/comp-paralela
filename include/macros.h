#ifndef MACROS_H
#define MACROS_H

#define deref_as(TYPE, ITEM) (*((TYPE*)(ITEM)))
#define MAX(A,B) ((A) > (B) ? (A) : (B))
#define MIN(A,B) ((A) < (B) ? (A) : (B))
#define u64 uint64_t
#define u32 uint32_t
#define i8 int8_t
#define not !
#define and &&
#define or ||
#define null ((void*)0)

#ifdef LOG_CODE_ENABLED
#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#define SHORT_FILE (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#define LOG(fmt, ...) printf("[LOG F%s L%d] -> " fmt "\n", SHORT_FILE, __LINE__, ##__VA_ARGS__)
#else
#define LOG(fmt, ...) 
#endif

#endif // MACROS_H