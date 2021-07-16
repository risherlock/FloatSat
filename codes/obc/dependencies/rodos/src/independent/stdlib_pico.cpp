
#include "stdlib_pico.h"

namespace RODOS {

bool isspace(const char c){
    //  htab, line feed, vtab, formfeed, carriage return, space)
    return ((c >= 0x09 && c <= 0x0D) || c == 0x20);
}

bool isdigit(const char c){
    return (c >= '0' && c <= '9');
}

bool isalpha(const char c){
    return ( (c>='a' && c<='z') || (c>='A' && c<='Z'));
}

bool isupper(const char c){
    return (c>='A' && c<='Z');
}


int32_t strToInt32_t (const char * nptr, const char ** endptr, uint8_t base) {
    const char *s = nptr;
    int32_t acc;
    char c;
    uint32_t cutoff;
    int neg = 0, any;
    uint8_t cutlim;

    /*
     * Skip white space and pick up leading +/- sign if any.
     * If base is 0, allow 0x for hex and 0 for octal, else
     * assume decimal; if base is already 16, allow 0x.
     */
    do {
        c = *s++;
    } while (isspace(c));
    if (c == '-') {
        neg = 1;
        c = *s++;
    } else if (c == '+')
        c = *s++;
    if ((base == 0u || base == 16u) &&
        c == '0' && (*s == 'x' || *s == 'X')) {
        c = s[1];
        s += 2;
        base = 16;
    }
    if (base == 0)
        base = c == '0' ? 8 : 10;

    /*
     * Compute the cutoff value between legal numbers and illegal
     * numbers.  That is the largest legal value, divided by the
     * base.  An input number that is greater than this value, if
     * followed by a legal input character, is too big.  One that
     * is equal to this value may be valid or not; the limit
     * between valid and invalid numbers is then based on the last
     * digit.  For instance, if the range for int32_t is
     * [-2147483648..2147483647] and the input base is 10,
     * cutoff will be set to 214748364 and cutlim to either
     * 7 (neg==0) or 8 (neg==1), meaning that if we have accumulated
     * a value > 214748364, or equal but the next digit is > 7 (or 8),
     * the number is too big, and we will return a range error.
     *
     * Set any if any `digits' consumed; make it negative to indicate
     * overflow.
     */
    cutoff = neg ? -static_cast<uint32_t>(INT32_MIN) : static_cast<uint32_t>(INT32_MAX);
    cutlim = static_cast<uint8_t>(cutoff % base);
    cutoff /= base;
    for (acc = 0, any = 0;; c = *s++) {
        if (isdigit(c))
            c = static_cast<char>(c - '0');
        else if (isalpha(c))
            c = static_cast<char>(c - (isupper(c) ? 'A' - 10 : 'a' - 10));
        else
            break;
        if (c >= base)
            break;
        if (any < 0 || acc > static_cast<int32_t>(cutoff) || (acc == static_cast<int32_t>(cutoff) && c > static_cast<int8_t>(cutlim)))
            any = -1;
        else {
            any = 1;
            acc *= base;
            acc += c;
        }
    }
    if (any < 0) {
        acc = neg ? INT32_MIN : INT32_MAX;
        //rptr->_errno = ERANGE;
    } else if (neg)
        acc = -acc;
    if (endptr != 0)
        *endptr = (const char *) (any ? (const char *)s - 1 : nptr);
    return (acc);
}


} // namespace

