#include <sys/types.h>
#include "rwlock.h"

#define MAX_OPEN_FILES 4093 // This is what linux uses. Ask if good number
#define OPEN_FILE_OFFSET 3  // To account for stdin, stdout, and stderr

// Is there a way to not do these typedefs?

struct list_head {
	struct list_head *next, *prev;
};

struct nk_fs_open_file_state {
  spinlock_t lock;

  struct list_head file_node;
    
  struct nk_fs  *fs;
  void          *file;
    
  size_t   position;
  int      flags;
};

typedef struct nk_fs_open_file_state * nk_fs_fd_t;
#define FS_BAD_FD ((nk_fs_fd_t)-1UL)

typedef struct _file_table{
  nk_fs_fd_t * table;
  struct nk_rwlock rwlock;
} file_table;

int init_file_table(size_t initial_cap);

int add_file(nk_fs_fd_t fd);

int get_index(nk_fs_fd_t fd);

void deinit_file_table();
