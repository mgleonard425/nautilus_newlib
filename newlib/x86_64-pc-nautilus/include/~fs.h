#include "spinlock.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <sys/stat.h>

#define MAX_OPEN_FILES 4096 // This is what linux uses. Ask if good number

struct nk_fs_open_file_state;
typedef struct nk_fs_open_file_state * nk_fs_fd_t;
#define FS_BAD_FD ((nk_fs_fd_t)-1UL)

struct ft_entry_ops {
  // Performable operations on a file table entry
  int (*close)(int file);
  int (*read)(int file, char *ptr, int len);
  int (*write)(int file, char *ptr, int len);
  int (*lseek)(int file, off_t position, int whence);
  int (*fstat)(int file, struct stat *st);
};

typedef struct _ft_entry{
  unsigned alloc : 1;   // 1 if allocated, 0 if free
  nk_fs_fd_t fd;
  struct ft_entry_ops * ops;
} ft_entry;

typedef struct _file_table{
  ft_entry * table;
  my_spinlock_t * lock;
} file_table;

int init_file_table();
int add_file(nk_fs_fd_t fd);
nk_fs_fd_t remove_file(int file);
int first_free_ft_entry();
void deinit_file_table();

static int stdin_close(int file);
static int stdout_close(int file);
static int stderr_close(int file);
static int file_close(int file);

static int stdin_read(int file, char *ptr, int len);
static int stdout_read(int file, char *ptr, int len);
static int stderr_read(int file, char *ptr, int len);
static int file_read(int file, char *ptr, int len);

static int stdin_write(int file, char *ptr, int len);
static int stdout_write(int file, char *ptr, int len);
static int stderr_write(int file, char *ptr, int len);
static int file_write(int file, char *ptr, int len);

static int stdin_lseek(int file, off_t position, int whence);
static int stdout_lseek(int file, off_t position, int whence);
static int stderr_lseek(int file, off_t position, int whence);
static int file_lseek(int file, off_t position, int whence);

static int stdin_fstat(int file, struct stat *st);
static int stdout_fstat(int file, struct stat *st);
static int stderr_fstat(int file, struct stat *st);
static int file_fstat(int file, struct stat *st);
