#include "include/fs.h"

#define FT_LOCK_CONF uint8_t _ft_lock_flags
#define FT_LOCK() _ft_lock_flags = my_spin_lock_irq_save(&(ft.lock))
#define FT_UNLOCK() my_spin_unlock_irq_restore(&(ft.lock), _ft_lock_flags)

static file_table ft;

/* The branching works as follows: */
/* A program executes one of these system calls */
/* The wrapper function checks that the file descriptor is valid */
/* If it is, it goes to the specified file's entry in the file table, */
/* and performs the behavior specified by its member function */
/* N.B. In this implementation, stdout and stderr behave identically */

///////////////////////////////////// open ///////////////////////////////////////

// We only do the internal version with the underscore thing
// because the newlib headers make us. Sorry!
int _open(const char *pathname, int flags, int mode) {
  nk_fs_fd_t fd = nk_fs_open(pathname, flags, mode);
  if (fd == FS_BAD_FD)
    return -1;
  return add_file(fd);
}

int open(const char *name, int flags, ...)
{
  if(!strncmp(name, "stdin", 5)) {
    nk_vc_printf("open syscall not implemented for stdin\n");
    return 0;
  }
  if(!strncmp(name, "stdout", 6)) {
    nk_vc_printf("open syscall not implemented for stdout\n");
    return 0;
  }
  if(!strncmp(name, "stderr", 6)) {
    nk_vc_printf("open syscall not implemented for stderr\n");
    return 0;
  }
  return _open(name, flags, 0);
}

//////////////////////////////////// close ///////////////////////////////////////

// Same as open. We only do it this way bc we have to. Sorry!

int _close(int file) {
  if (file < 0 || file > MAX_OPEN_FILES) {
    nk_vc_printf("Invalid file descriptor\n");
    return -1;
  }
  return ft.table[file].ops.close(file);
}

int close(int file)
{
  return _close(file);
}

static int stdin_close(int file) {
  nk_vc_printf("close syscall not implemented for stdin\n");
  return 0;
}

static int stdout_close(int file) {
  nk_vc_printf("close syscall not implemented for stdout\n");
  return 0;
}

static int stderr_close(int file) {
  nk_vc_printf("close syscall not implemented for stderr\n");
  return 0;
}

static int file_close(int file) {
  nk_fs_fd_t fd = remove_file(file);
  if(fd == FS_BAD_FD) {
    nk_vc_printf("Tried to close an nonexistent or non-open file\n");
    return -1;
  }
  nk_fs_close(fd);
  return 0;
}

///////////////////////////////////// read ///////////////////////////////////////

int read(int file, char *ptr, int len)
{
  if (file < 0 || file > MAX_OPEN_FILES) {
    nk_vc_printf("Invalid file descriptor\n");
    return -1;
  }
  return ft.table[file].ops.read(file, ptr, len);
}

static int stdin_read(int file, char *ptr, int len){
  // WTF am I supposed to do with this?
  nk_vc_printf("WTF am I supposed to do with a read from stdin?\n");
  return 0;
  // Sorry
}

static int stdout_read(int file, char *ptr, int len){
  nk_vc_printf("read not implemented for stdout\n");
  return 0;
}

static int stderr_read(int file, char *ptr, int len){
  nk_vc_printf("read not implemented for stderr\n");
  return 0;
}

static int file_read(int file, char *ptr, int len){
  nk_fs_fd_t fd = ft.table[file].fd;
  return nk_fs_read(fd, (void *) ptr, len);
}

//////////////////////////////////// write ///////////////////////////////////////

int write(int file, char *ptr, int len)
{
  if (file < 0 || file > MAX_OPEN_FILES) {
    nk_vc_printf("Invalid file descriptor\n");
    return -1;
  }
  return ft.table[file].ops.write(file, ptr, len);
}

static int stdin_write(int file, char *ptr, int len) {
  nk_vc_printf("Write syscall not implemented for stdin\n");
  return 0;
}

static int stdout_write(int file, char *ptr, int len) {
  // Am I supposed to do something to make sure we print to the correct virtual console?
  nk_vc_printf("%.*s", len, ptr);
  return 0;
}

static int stderr_write(int file, char *ptr, int len) {
  // Treat stdout and stderr identically for now...
  nk_vc_printf("%.*s", len, ptr);
  return 0;
}

static int file_write(int file, char *ptr, int len) {
  nk_fs_fd_t fd = ft.table[file].fd;
  return nk_fs_write(fd, (void *) ptr, len);
}

///////////////////////////////////// lseek //////////////////////////////////////

int lseek(int file, off_t position, int whence) {
  if (file < 0 || file > MAX_OPEN_FILES) {
    nk_vc_printf("Invalid file descriptor\n");
    return -1;
  }
  return ft.table[file].ops.lseek(file, position, whence);
}

static int stdin_lseek(int file, off_t position, int whence) {
  nk_vc_printf("lseek not implemented for stdin\n");
  return 0;
}

static int stdout_lseek(int file, off_t position, int whence) {
  nk_vc_printf("lseek not implemented for stdout\n");
  return 0;
}

static int stderr_lseek(int file, off_t position, int whence) {
  nk_vc_printf("lseek not implemented for stderr\n");
  return 0;
}

static int file_lseek(int file, off_t position, int whence) {
  nk_fs_fd_t fd = ft.table[file].fd;
  return nk_fs_seek(fd, position, whence);
}

///////////////////////////////////// stat //////////////////////////////////////

int stat(const char *path, struct stat *st) {
  int file = open(path, O_RDONLY);
  if (file < 3) {
    nk_vc_printf("Failed to open file for stat\n");
    return -1;
  }
  return fstat(file, st);
}

int lstat(const char *path, struct stat *st) {
  // For now, this is exactly the same as stat
  // If symlinks are ever implemented in nautilus, this must be modified to account for them
  int file = open(path, O_RDONLY);
  if (file < 3) {
    nk_vc_printf("Failed to open file for stat\n");
    return -1;
  }
  return fstat(file, st);
}

int fstat(int file, struct stat *st) {
  if (file < 0 || file > MAX_OPEN_FILES) {
    nk_vc_printf("Invalid file descriptor\n");
    return -1;
  }
  return ft.table[file].ops.fstat(file, st);
}

static int stdin_fstat(int file, struct stat *st) {
  nk_vc_printf("fstat not implemented for stdin\n");
  return 0;
}

static int stdout_fstat(int file, struct stat *st) {
  nk_vc_printf("fstat not implemented for stdout\n");
  return 0;
}

static int stderr_fstat(int file, struct stat *st) {
  nk_vc_printf("fstat not implemented for stderr\n");
  return 0;
}

static int file_fstat(int file, struct stat *st) {
  nk_fs_fd_t fd = ft.table[file].fd;
  struct nk_fs_stat nk_stat;
  int rc = nk_fs_fstat(fd, &nk_stat);
  st->st_size = nk_stat.st_size;
  return rc;
}

///////////////////////////////////// dup2 //////////////////////////////////////

int dup2(int oldfd, int newfd) {
  if (oldfd < 0 || oldfd > MAX_OPEN_FILES || newfd < 0 || newfd > MAX_OPEN_FILES) {
    nk_vc_printf("Invalid file descriptor\n");
    return -1;
  }

  if(newfd == 0 || newfd == 1 || newfd == 2) {
    nk_vc_printf("Error: can't write over stdin, stdout, or stderr\n");
    return -1;
  }

  if(ft.table[oldfd].alloc) {
    int close_rc = close(oldfd);
    if (close_rc) {
      nk_vc_printf("Tried to call dup2 with newfd as a currently open file. Failed to close newfd\n");
    }
  }

  ft.table[newfd] = ft.table[oldfd];
}


///////////////////////////////////////////////////////////////////////////////////
//                                                                               //
//                                  Internal                                     //
//                                                                               //
///////////////////////////////////////////////////////////////////////////////////

int init_file_table() {
  // Zero out all the file table entries
  memset(&ft, 0, MAX_OPEN_FILES*sizeof(ft_entry));

  // Initialization of stdin file table entry
  ft.table[0].ops.close = stdin_close;
  ft.table[0].ops.read = stdin_read;
  ft.table[0].ops.write = stdin_write;
  ft.table[0].ops.lseek = stdin_lseek;
  ft.table[0].ops.fstat = stdin_fstat;

  // Initialization of stdout file table entry
  ft.table[1].ops.close = stdout_close;
  ft.table[1].ops.read = stdout_read;
  ft.table[1].ops.write = stdout_write;
  ft.table[1].ops.lseek = stdout_lseek;
  ft.table[1].ops.fstat = stdout_fstat;

  // Initialization of stderr file table entry
  ft.table[2].ops.close = stderr_close;
  ft.table[2].ops.read = stderr_read;
  ft.table[2].ops.write = stderr_write;
  ft.table[2].ops.lseek = stderr_lseek;
  ft.table[2].ops.fstat = stderr_fstat;

  // For now, all files other than 0,1,2 are normal files
  // Therefore, their operations follow standard behavior
  for (int i = 3; i < MAX_OPEN_FILES; ++i) {
    ft.table[i].ops.close = file_close;
    ft.table[i].ops.read = file_read;
    ft.table[i].ops.write = file_write;
    ft.table[i].ops.lseek = file_lseek;
    ft.table[i].ops.fstat = file_fstat;
  }

  my_spinlock_init(&(ft.lock));
  return 0;
}

int first_free_ft_entry() {
  for (int i = 3; i < MAX_OPEN_FILES; ++i) {
    if (!(ft.table[i].alloc))
      return i;
  }
  return -1;
}

int add_file(nk_fs_fd_t fd) {
  FT_LOCK_CONF;

  int i = first_free_ft_entry();
  if(i < 0) return i;    // If i = -1, the file table must be full

  FT_LOCK();
  ft.table[i].fd = fd;
  ft.table[i].alloc = 1;
  FT_UNLOCK();

  return i;
}

nk_fs_fd_t remove_file(int file) {
  FT_LOCK_CONF;

  FT_LOCK();
  if(ft.table[file].alloc == 0) {
    nk_vc_printf("File descriptor %d does not correspond to an open file\n", file);
    return FS_BAD_FD;
  }
  ft.table[file].alloc = 0;
  FT_UNLOCK();

  return ft.table[file].fd;
}
