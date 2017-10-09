/* We need a wrapper function for one of the additions of POSIX.  */
int
__posix_memalign (void **memptr, size_t alignment, size_t size)
{
  void *mem;

  /* Test whether the ALIGNMENT argument is valid.  It must be a power
     of two multiple of sizeof (void *).  */
  if (alignment % sizeof (void *) != 0 || (alignment & (alignment - 1)) != 0)
    return EINVAL;

  mem = __libc_memalign (alignment, size);

  if (mem != NULL)
    {
      *memptr = mem;
      return 0;
    }

  return ENOMEM;
}
weak_alias (__posix_memalign, posix_memalign)
