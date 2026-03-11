#include "libc.h"

void delay(uint32_t a) {
    volatile uint32_t i;
    for (i = 0; i < a; i++)
        ;
}

void *memset(void *str, int c, size_t n) {
    uint8_t *s = str;
    for (size_t i = 0; i < n; i++) {
        *s = c;
        s++;
    }
    return str;
}

// Check if pointers are not 4-byte aligned
#define UNALIGNED(X, Y) (((uint32_t)(X) & (sizeof(uint32_t) - 1U)) | ((uint32_t)(Y) & (sizeof(uint32_t) - 1U)))

// Optimized memcpy: if src/dest are word-aligned, copy 4 bytes at a time (4x faster on Cortex-M4).
// 16-byte unrolled loop reduces overhead for large copies. Falls back to byte copy for unaligned/remainder.
void *memcpy(void *dest, const void *src, size_t len) {
    size_t n = len;
    uint8_t *d8 = dest;
    const uint8_t *s8 = src;

    if ((n >= 4U) && !UNALIGNED(s8, d8)) {
        uint32_t *d32 = (uint32_t *)d8;
        const uint32_t *s32 = (const uint32_t *)s8;

        while (n >= 16U) {
            *d32 = *s32;
            d32++;
            s32++;
            *d32 = *s32;
            d32++;
            s32++;
            *d32 = *s32;
            d32++;
            s32++;
            *d32 = *s32;
            d32++;
            s32++;
            n -= 16U;
        }

        while (n >= 4U) {
            *d32 = *s32;
            d32++;
            s32++;
            n -= 4U;
        }

        d8 = (uint8_t *)d32;
        s8 = (const uint8_t *)s32;
    }
    while (n-- > 0U) {
        *d8 = *s8;
        d8++;
        s8++;
    }
    return dest;
}

int memcmp(const void *ptr1, const void *ptr2, size_t num) {
    const uint8_t *p1 = ptr1;
    const uint8_t *p2 = ptr2;
    for (size_t i = 0; i < num; i++) {
        if (*p1 != *p2) {
            return (*p1 < *p2) ? -1 : 1;
        }
        p1++;
        p2++;
    }
    return 0;
}

size_t strlen(const char *str) {
    size_t len = 0;
    while (*str++) {
        len++;
    }
    return len;
}
