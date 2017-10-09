#include "include/spinlock.h"
/* #include <nautilus/mm.h> */
/* #include <nautilus/spinlock.h> */

#undef malloc
#undef free

void * kmem_malloc(size_t);
void free(void *);
void * memset(void *, int, size_t);
void * memcpy(void *, const void *, size_t);

// Make this elegant later
void * malloc(size_t size) {
  return kmem_malloc(size);
}

void free(void * block) {
  kmem_free(block);
}

void * calloc(size_t nmemb, size_t size) {
  void * block = malloc(nmemb*size);
  if(block != NULL) {
    memset(block, 0, nmemb*size);
  }
  return block;
}

void * realloc(void * ptr, size_t size) {
  void * new_block = malloc(size);
  if(new_block != NULL) {
    memcpy(new_block, ptr, size);  // USE KMEM_FIND_BLOCK
    free(ptr);
  }
  return new_block;
}

void * _malloc_r(struct _reent * reent, size_t size) {
  return malloc(size);
}

void _free_r(struct _reent * reent, void * block) {
  free(block);
}

void * _calloc_r(struct _reent * reent, size_t nmemb, size_t size) {
  return calloc(nmemb, size);
}

void * _realloc_r(struct _reent * reent, void * ptr, size_t size) {
  return realloc(ptr, size);
}

void 
my_spinlock_init (volatile my_spinlock_t * lock) {
  spinlock_init(lock);
}

void
my_spinlock_deinit (volatile my_spinlock_t * lock) {
  spinlock_init(lock);
}

void
my_spin_lock (volatile my_spinlock_t * lock) {
  spin_lock(lock);
}

int
my_spin_try_lock(volatile my_spinlock_t *lock) {
  return spin_try_lock(lock);
}

uint8_t
my_spin_lock_irq_save (volatile my_spinlock_t * lock) {
  return spin_lock_irq_save(lock);
}

void
my_spin_lock_nopause (volatile my_spinlock_t * lock) {
  spin_lock_nopause(lock);
}

uint8_t
my_spin_lock_irq_save_nopause (volatile my_spinlock_t * lock) {
  return spin_lock_irq_save_nopause(lock);
}

void 
my_spin_unlock (volatile my_spinlock_t * lock) {
  spin_unlock(lock);
}

void
my_spin_unlock_irq_restore (volatile my_spinlock_t * lock, uint8_t flags) {
  spin_unlock_irq_restore(lock, flags);
}
