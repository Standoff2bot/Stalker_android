#pragma once

// Android compatibility layer for OpenXRay
// Provides missing functions that would normally come from SDL2

#include <cstdlib>
#include <cstring>
#include <cstdio>

// Simple itoa implementation for Android (replaces SDL_itoa)
inline char* android_itoa(int value, char* str, int base) {
    if (base < 2 || base > 36) {
        *str = '\0';
        return str;
    }
    
    char* ptr = str;
    char* ptr1 = str;
    char tmp_char;
    int tmp_value;
    
    // Handle negative numbers for base 10
    if (value < 0 && base == 10) {
        *ptr++ = '-';
        ptr1++;
        value = -value;
    }
    
    // Convert to string in reverse order
    do {
        tmp_value = value;
        value /= base;
        *ptr++ = "0123456789abcdefghijklmnopqrstuvwxyz"[tmp_value - value * base];
    } while (value);
    
    *ptr-- = '\0';
    
    // Reverse string
    while (ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr-- = *ptr1;
        *ptr1++ = tmp_char;
    }
    
    return str;
}

// Simple strupr implementation (converts string to uppercase)
inline char* android_strupr(char* str) {
    char* ptr = str;
    while (*ptr) {
        if (*ptr >= 'a' && *ptr <= 'z') {
            *ptr = *ptr - ('a' - 'A');
        }
        ptr++;
    }
    return str;
}

// Define macros for OpenXRay code
#define itoa android_itoa
#define _itoa_s(val, buf, bufsize, base) android_itoa(val, buf, base)
#define SDL_itoa android_itoa
#define SDL_strupr android_strupr
#define strupr android_strupr
