#ifndef LIBC_H
#define LIBC_H

#include <stddef.h>
#include <stdint.h>

void delay(uint32_t a);
void *memset(void *str, int c, size_t n);
void *memcpy(void *dest, const void *src, size_t len);
int memcmp(const void *ptr1, const void *ptr2, size_t num);
size_t strlen(const char *str);

#endif // LIBC_H
