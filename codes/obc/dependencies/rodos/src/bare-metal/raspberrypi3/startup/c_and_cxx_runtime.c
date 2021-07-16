// Lizenz: public domain
#include <stdint.h>

typedef void (*constructor_t)(void);

// Im Linkerskript definiert
extern constructor_t __init_array_start;
extern constructor_t __init_array_end;

void call_constructors(void) {
    constructor_t* fn = &__init_array_start;
    while(fn < &__init_array_end) {
        (*fn)();
        fn++;
    }
}

#ifndef __USE_GLOBAL_CXXABI__
/**
 * Code from Andre Bartke <dev@bartke.cc>
 */

// Guard type for C++ ABI
typedef int64_t __guard;

typedef struct {
    int64_t counter;
} atomic_t;

typedef enum {
    INIT_DONE,
    INIT_PENDING,
    INIT_NONE
} mstate_t;


int __aeabi_atexit(void* object, void (*destructor)(void*), void* dso_handle) {
    (void)object;
    (void)destructor;
    (void)dso_handle;
    return 0;
}

void* __dso_handle = 0;


int __cxa_atexit(void (*f)(void*), void* p, void* d) {
    (void)f;
    (void)p;
    (void)d;
    return 0;
}

void __cxa_pure_virtual(void) {
    while(1) {}
}

void __cxa_finalize(void* p) {
    (void)p;
    // unnecessary
}

/**
 * @brief Atomic compare and exchange.
 *
 * @param v volatile memory
 * @param o old value
 * @param n new value
 *
 * @retval old value; test if successful by comparing old with return value
 */
static inline int64_t atomic_cmpxchg(atomic_t* v, int64_t o, int64_t n) {
    int64_t  prev;
    uint32_t flags;

    asm volatile(
      "    mrs    %0, cpsr\n"
      "    cpsid    i"
      : "=r"(flags)
      :
      : "memory", "cc");

    prev = v->counter;
    if(prev == o)
        v->counter = n;

    asm volatile(
      " msr cpsr_c, %0"
      :
      : "r"(flags)
      : "memory", "cc");

    return prev;
}

/**
 * @brief Acquire guard to ensure thread safety for static local variables.
 * This function is called before initialization takes place. If this
 * function returns 1, either __cxa_guard_release or __cxa_guard_abort must
 * be called with the same argument. The first byte of the guard_object is
 * not modified by this function.
 *
 * @param g 64-bit __guard variable
 *
 * @retval 1 if the initialization is not yet completed
 * @retval 0 initialization is completed
 */
int __cxa_guard_acquire(__guard* g) {
    atomic_t* m = (atomic_t*)g;

    /* check that the initializer has not already been run */
    if(m->counter == INIT_DONE)
        return 0;

    /* acquire a look:
    * if possible, we are ready to initialize, set state to PENDING and
    * return 1
    */
    if(INIT_NONE == atomic_cmpxchg(m, INIT_NONE, INIT_PENDING))
        return 1;
    else {
        /* if not possible, wait for completion of the other thread */
        while(m->counter == INIT_PENDING)
            ;
        ;
        return 0;
    }
}

/**
 * @brief Mark __guard as initialized.
 *
 * @param g 64-bit __guard variable
 */
void __cxa_guard_release(__guard* g) {
    atomic_t* m = (atomic_t*)g;
    m->counter  = INIT_DONE;
}

/**
 * @brief Mark __guard as uninitialized.
 *
 * @param g 64-bit __guard variable
 */
void __cxa_guard_abort(__guard* g) {
    atomic_t* m = (atomic_t*)g;
    m->counter  = INIT_NONE;
}
#endif
