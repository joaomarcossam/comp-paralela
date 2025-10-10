#ifndef MACROS_H
#define MACROS_H

#define u32 uint32_t
#define i8 int8_t
#define deref_as(TYPE, ITEM) (*((TYPE*)(ITEM)))
#define not !
#define and &&
#define or ||
#define null ((void*)0)

#ifdef LOG_CODE_ENABLED
#include <stdio.h>
#define LOG(fmt, ...) printf("[LOG F%s L%d] -> " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#else
#define LOG(fmt, ...) 
#endif

#endif // MACROS_H