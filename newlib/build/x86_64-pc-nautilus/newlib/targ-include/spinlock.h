#include <stdint.h>
#include <stdlib.h>

typedef uint32_t my_spinlock_t;

void 
my_spinlock_init (volatile my_spinlock_t * lock);

void
my_spinlock_deinit (volatile my_spinlock_t * lock);

void
my_spin_lock (volatile my_spinlock_t * lock);

int
my_spin_try_lock(volatile my_spinlock_t *lock);

uint8_t
my_spin_lock_irq_save (volatile my_spinlock_t * lock);

void
my_spin_lock_nopause (volatile my_spinlock_t * lock);

uint8_t
my_spin_lock_irq_save_nopause (volatile my_spinlock_t * lock);

void 
my_spin_unlock (volatile my_spinlock_t * lock);

void
my_spin_unlock_irq_restore (volatile my_spinlock_t * lock, uint8_t flags);

void * malloc(size_t size);

void free(void *);

void * calloc(size_t nmemb, size_t size);

void * realloc(void * ptr, size_t size);

