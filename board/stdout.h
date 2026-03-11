#ifndef STDOUT_H
#define STDOUT_H

#include "libc.h"
#include "stm32f4xx_hal.h"
#include <stdarg.h>
#include <stdint.h>

#define UART_PRINT_BUFFER_SIZE 128

extern UART_HandleTypeDef huart5;

// Convert int to string. Magic string: index 35 = '0', left = 1-9,a-z, right = same.
// "35 + remainder" maps any digit (base 2-36) to correct char.
static inline void itoa(int value, char *str, int base) {
    char *ptr = str, *ptr1 = str, tmp_char;
    int tmp_value;

    if (base == 10 && value < 0) {
        value = -value;
        *ptr++ = '-';
    }
    ptr1 = ptr;

    do {
        tmp_value = value;
        value /= base;
        *ptr++ =
            "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + (tmp_value - value * base)];
    } while (value);

    *ptr-- = '\0';

    while (ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr-- = *ptr1;
        *ptr1++ = tmp_char;
    }
}

static inline void utoa(unsigned int value, char *str, int base) {
    char *ptr = str, *ptr1 = str, tmp_char;
    unsigned int tmp_value;

    do {
        tmp_value = value;
        value /= (unsigned int)base;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"
            [35 + (tmp_value - value * (unsigned int)base)];
    } while (value);

    *ptr-- = '\0';

    while (ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr-- = *ptr1;
        *ptr1++ = tmp_char;
    }
}

static inline void ultoa(unsigned long value, char *str, int base) {
    char *ptr = str, *ptr1 = str, tmp_char;
    unsigned long tmp_value;

    do {
        tmp_value = value;
        value /= (unsigned long)base;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"
            [35 + (int)(tmp_value - value * (unsigned long)base)];
    } while (value);

    *ptr-- = '\0';

    while (ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr-- = *ptr1;
        *ptr1++ = tmp_char;
    }
}

static inline void float_to_string(char *buffer, double value, int decimalPlaces) {
    if (buffer == NULL || decimalPlaces < 0)
        return;

    if (value < 0) {
        *buffer++ = '-';
        value = -value;
    }

    double roundingFactor = 0.5;
    for (int i = 0; i < decimalPlaces; ++i) {
        roundingFactor /= 10.0;
    }
    value += roundingFactor;

    long intPart = (long)value;
    value -= intPart;

    char intBuffer[20];
    char *intPtr = intBuffer;
    do {
        *intPtr++ = (intPart % 10) + '0';
        intPart /= 10;
    } while (intPart > 0);

    char *intStart = intBuffer;
    char *intEnd = intPtr - 1;
    while (intStart < intEnd) {
        char temp = *intStart;
        *intStart++ = *intEnd;
        *intEnd-- = temp;
    }

    while (intPtr > intBuffer) {
        *buffer++ = *--intPtr;
    }

    if (decimalPlaces > 0) {
        *buffer++ = '.';

        for (int i = 0; i < decimalPlaces; i++) {
            value *= 10;
            int digit = (int)value;
            *buffer++ = digit + '0';
            value -= digit;
        }
    }

    *buffer = '\0';
}

static inline void debug_double(double value) {
    char buffer[32];
    float_to_string(buffer, value, 2);
    HAL_UART_Transmit(&huart5, (uint8_t *)buffer, strlen(buffer), HAL_MAX_DELAY);
}

// Lightweight printf to UART. Supports: %c %d %u %x %X %s %f, width specifiers, %l modifier.
// ~260 lines vs ~20KB for stdlib printf. Outputs directly to UART5.
static inline void uart_printf(const char *fmt, ...) {
    static char buffer[UART_PRINT_BUFFER_SIZE];
    char *buf_ptr = buffer;
    char *buf_end = buffer + UART_PRINT_BUFFER_SIZE - 1;
    const char *traverse;
    va_list arg;
    va_start(arg, fmt);

    for (traverse = fmt; *traverse != '\0' && buf_ptr < buf_end; traverse++) {
        if (*traverse != '%') {
            *buf_ptr++ = *traverse;
            continue;
        }

        traverse++;

        int width = 0;
        char pad_char = ' ';

        if (*traverse == '0') {
            pad_char = '0';
            traverse++;
        }

        while (*traverse >= '0' && *traverse <= '9') {
            width = width * 10 + (*traverse - '0');
            traverse++;
        }

        int long_mod = 0;
        if (*traverse == 'l') {
            long_mod = 1;
            traverse++;
        }

        switch (*traverse) {
        case 'c': {
            char c = va_arg(arg, int);
            if (buf_ptr < buf_end) {
                *buf_ptr++ = c;
            }
            break;
        }
        case 'd': {
            char str[12];
            if (long_mod) {
                long li = va_arg(arg, long);
                itoa((int)li, str, 10);
            } else {
                int i = va_arg(arg, int);
                itoa(i, str, 10);
            }
            for (char *p = str; *p && buf_ptr < buf_end; p++) {
                *buf_ptr++ = *p;
            }
            break;
        }
        case 'u': {
            char str[12];
            if (long_mod) {
                unsigned long ul = va_arg(arg, unsigned long);
                ultoa(ul, str, 10);
            } else {
                unsigned int ui = va_arg(arg, unsigned int);
                utoa(ui, str, 10);
            }
            for (char *p = str; *p && buf_ptr < buf_end; p++) {
                *buf_ptr++ = *p;
            }
            break;
        }
        case 'x':
        case 'X': {
            char str[16];
            if (long_mod) {
                unsigned long ul = va_arg(arg, unsigned long);
                ultoa(ul, str, 16);
            } else {
                unsigned int ui = va_arg(arg, unsigned int);
                utoa(ui, str, 16);
            }
            if (*traverse == 'X') {
                for (char *p = str; *p; p++) {
                    if (*p >= 'a' && *p <= 'f') {
                        *p -= 32;
                    }
                }
            }
            int len = strlen(str);
            while (len < width && buf_ptr < buf_end) {
                *buf_ptr++ = pad_char;
                len++;
            }
            for (char *p = str; *p && buf_ptr < buf_end; p++) {
                *buf_ptr++ = *p;
            }
            break;
        }
        case 's': {
            char *s = va_arg(arg, char *);
            while (*s && buf_ptr < buf_end) {
                *buf_ptr++ = *s++;
            }
            break;
        }
        case 'f': {
            double d = va_arg(arg, double);
            char str[32];
            float_to_string(str, d, 2);
            for (char *p = str; *p && buf_ptr < buf_end; p++) {
                *buf_ptr++ = *p;
            }
            break;
        }
        default:
            if (buf_ptr < buf_end) {
                *buf_ptr++ = *traverse;
            }
            break;
        }
    }

    *buf_ptr = '\0';
    va_end(arg);

    HAL_UART_Transmit(&huart5, (uint8_t *)buffer, buf_ptr - buffer, HAL_MAX_DELAY);
}

#endif // STDOUT_H
