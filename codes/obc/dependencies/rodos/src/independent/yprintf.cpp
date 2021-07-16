/**
 * micro printf (subset)
 *
 * Author: Unkown (long long time ago)
 * c++, %ll, %b %f: Extensions: Sergio Montenegro, 2011
 * %e Extensions: Michael Strohmeier 2018
 *
 */
#include <stdarg.h>


#include "rodos-debug.h"
#include "yprintf.h"


namespace RODOS {

void Yprintf::vaprintf(const char *fmt) {
    char c;

    while ((c = *fmt++) != 0) {
        unsigned char is_signed = 0;
        unsigned char is_long = 0;
        unsigned char is_longlong = 0;
        unsigned char is_float = 0;
        unsigned char is_exp = 0;
        unsigned short base;
        char *ptr;

        if (c != '%') {
            yputc(c);
            continue;
        }
        c = *fmt++;

        char width = 0;
        char fill = ' ';
        char decimalPos = 3;

        if (c == '0') {
            fill = c;
            c = *fmt++;
        }
        if (c >= '0' && c <= '9') {
            width = static_cast<char>(c - '0');
            c = *fmt++;
        }
        if (c == '.') {
            c = *fmt++; if(c == 0) return; // SM: bad format
            decimalPos = static_cast<char>(c - '0');
            c = *fmt++; if(c == 0) return; // SM: bad format
            // allow more than 9 decimalPos
            while(c >= '0' && c <= '9') {
                decimalPos = static_cast<char>(decimalPos*10 + (c - '0'));
                c = *fmt++;
            }
        }

        if (c == 'l') {
            is_long = 1;
            c = *fmt++;
        }
        if (c == 'l') {
            is_longlong = 1;
            c = *fmt++;
        }
        if(c == 0) return; //SM: Bad format eg "cnt =%" or "%3" etc

        switch (c) {
        case 's':
            ptr = va_arg(ap, char *);
            while ((c = *ptr++)) {
                yputc(c);
            }
            continue;

        case 'o':
            base = 8;
            yputc('0');
            break;

        case 'e':
            is_exp = 1;
            is_float = 1;
            is_signed = 1;
            base = 10;
            break;

        case 'f':
            is_float = 1;
            is_signed = 1;
            base = 10;
            break;

        case 'd':
            is_signed = 1;
            base = 10;
            break;

        case 'i':
            is_signed = 0;
            base = 10;
            break;

        case 'u':
            base = 10;
            break;

        case 'x':
            base = 16;
            break;

        case 'b':
            base = 2;
            break;

        default:
            if (c == 'c') {
                c = static_cast<char>(va_arg(ap, int));	// char promoted to int
            }
            yputc(c);
            continue;
        }

        // Number output
        {
            unsigned long long u_val;
            long long s_val = 0;
            double    f_val = 0.0;
            long      e_val = 0;

            {
                if (is_exp) {
                    f_val = va_arg(ap, double);
                    if(f_val != f_val) {
                        yputc('n'); yputc('a'); yputc('n');
                        continue;
                    }
                    signed char sign = 1;
                    double tmp = 1;
                    int pow = width - 1;

                    if(f_val < 0) {
                        sign = -1;
                    }


                    if(sign == -1 && pow > 0) {
                        pow--;
                    }


                    while(pow-- > 0) tmp *= 10;

                    if(sign*f_val < tmp) {
                        while(sign*f_val < tmp && f_val != 0 && e_val >= -306) {
                            f_val *= 10;
                            e_val--;
                        }
                    } else {
                        while(sign*f_val >= tmp && e_val <= 306) {
                            f_val /= 10;
                            e_val++;
                        }
                        f_val *= 10;
                        e_val--;
                    }

                    if(e_val <= -306) {
                        e_val = 0;
                        s_val = 0;
                        f_val = 0;
                    } else if (e_val >= 306) {
                        if(sign < 0) yputc('-');
                        yputc('i');
                        yputc('n');
                        yputc('f');
                        continue;
                    } else {
                        s_val = (long long) f_val;
                    }

                } else if (is_float) {
                    f_val = va_arg(ap, double);
                    if(f_val != f_val) {
                        yputc('n'); yputc('a'); yputc('n');
                        continue;
                    }
                    s_val = (long long) f_val;
                } else if (is_longlong) {
                    s_val = va_arg(ap, long long);
                } else if (is_long) {
                    s_val = va_arg(ap, long);
                } else {
                    s_val = va_arg(ap, int);
                }

                if (is_signed) {
                    if (s_val < 0 || f_val < 0) {
                        s_val = -s_val;		// make it positive
                        f_val = -f_val;
                        yputc('-');
                        if (width) {
                            --width;
                        }
                    }
                }

                if (is_longlong) {
                    u_val = (unsigned long long) s_val;
                } else if (is_long) {
                    u_val = (unsigned long long) (unsigned long) s_val;
                } else {
                    u_val = (unsigned long long) (unsigned int) s_val;
                }
            }

            {
                constexpr size_t SCRATCH_SIZE = 26;
                char             scratch[SCRATCH_SIZE];
                ptr  = &scratch[SCRATCH_SIZE-1];
                *ptr = 0;
                do {
                    char ch = (char) ((u_val % base) + '0');

                    if (ch > '9') {
                        ch = static_cast<char>(ch + 'A' - '9' - 1);
                    }

                    *--ptr = ch;
                    u_val /= base;

                    if (width) {
                        --width;
                    }
                } while (u_val);

                // insert padding chars
                while (width--) {
                    *--ptr = fill;
                }

                // output result
                while ((c = *ptr++)) {
                    yputc(c);
                }
            }

            // floating point
            if (is_float) {
                yputc('.');
                while(decimalPos--) {
                    f_val -= static_cast<double>(s_val);   // rm integer part
                    f_val = f_val * 10;
                    s_val = static_cast<long long>(f_val);
                    yputc(static_cast<char>(s_val + '0'));
                }
            }

            // exponential
            if (is_exp) {
                yputc('e');
                if(e_val < 0) {
                    yputc('-');
                    e_val *= -1;
                }

                constexpr size_t SCRATCH_SIZE = 6;
                char             scratch[SCRATCH_SIZE]; // biggest exponent 306: 3 digital positions
                ptr  = &scratch[SCRATCH_SIZE - 1];
                *ptr = 0;
                do {
                    char ch = (char) (e_val % base + '0');

                    *--ptr = ch;
                    e_val /= base;

                } while (e_val);

                // output result
                while ((c = *ptr++)) {
                    yputc(c);
                }
            }

        } // number outputs
    } // for all chars in format (fmt)
}

/*******************/
void xprintf(const char *fmt, ...) {
    if(printfMask == 0) return;
    Yprintf yprintf;
    va_start(yprintf.ap, fmt);
    yprintf.vaprintf(fmt);
}

void xsprintf(char* dest, const char *fmt, ...) {
    Ysprintf ysprintf(dest);
    va_start(ysprintf.ap, fmt);
    ysprintf.vaprintf(fmt);
}

} // namespace

