#ifndef _SYS_DIRENT_H
#define _SYS_DIRENT_H

#include <_ansi.h>

typedef long off_t;

typedef struct {
	int dd_fd;		/* Directory file. */
	int dd_loc;		/* Position in buffer. */
	int dd_seek;
	char *dd_buf;	/* Pointer to buffer. */
	int dd_len;		/* Buffer size. */
	int dd_size;	/* Data size in buffer. */
} DIR;

# define __dirfd(dp)	((dp)->dd_fd)

#ifndef MAXNAMLEN
#define MAXNAMLEN 1024      // max length for path name
                            // Use 1024 for now bc no restrictions in nautilus
#endif

DIR *opendir (const char *);
struct dirent *readdir (DIR *);
int readdir_r (DIR *__restrict, struct dirent *__restrict,
               struct dirent **__restrict);
void rewinddir (DIR *);
int closedir (DIR *);

#define d_fileno d_ino   // Backward compatibility

struct dirent {
	off_t		d_off;
	unsigned long	d_fileno;
	unsigned short	d_reclen;
	unsigned short	d_namlen;
	char		d_name[MAXNAMLEN + 1];
};

#endif
