#include <test/newlib_fs.h>

void open_simple_file() {
  nk_vc_printf("About to open foo.txt\n");
  int open_rc = open("/foo.txt", O_RDONLY);
  /* nk_fs_fd_t open_rc = nk_fs_open("/foo.txt", O_RDONLY, 0); */
  if(open_rc < 0) {
    nk_vc_printf("Failed to open foo.txt\n");
  } else {
    nk_vc_printf("Successfully opened foo.txt\n");
    nk_vc_printf("File descriptor: %d\n", open_rc);
  }

  nk_vc_printf("About to close foo.txt\n");
  int close_rc = close(open_rc);
  /* int close_rc = nk_fs_close(open_rc); */
  if(close_rc) {
    nk_vc_printf("Failed to close foo.txt\n");
  } else {
    nk_vc_printf("Successfully closed foo.txt\n");
  }
}

void check_function_pointers() {
  nk_vc_printf("Checking to see if function pointer branching works\n");
  nk_vc_printf("We're going to try closing a nonexistent file\n");
  nk_vc_printf("Expected behavior: error printout, nothing else\n");
  int close_rc = close(100);
  if(!close_rc) {
    nk_vc_printf("Something very wrong is happening\n");
  } else {
    nk_vc_printf("Everything seems fine\n");
  }
}

void open_and_read() {
  nk_vc_printf("Attempting to open bar.txt\n");
  int open_rc = open("/bar.txt", O_RDONLY);
  if (open_rc < 0) {
    nk_vc_printf("Failed to open bar.txt\n");
  } else {
    nk_vc_printf("Successfully opened bar.txt\n");
    nk_vc_printf("File descriptor: %d\n", open_rc);
  }

  char buf[3];
  nk_vc_printf("Attempting to read first three chars of bar.txt\n");
  nk_vc_printf("Expecting:\n");
  nk_vc_printf("bar\n");
  nk_vc_printf("Received:\n");

  int bytes_read = read(open_rc, buf, 3);
  for(int j = 0; j < 3; ++j) {
    nk_vc_printf("%c", buf[j]);
  }

  nk_vc_printf("\n");
}

void open_read_write() {
  nk_vc_printf("Attempting to open bar.txt\n");
  int open_rc = open("/bar.txt", O_RDONLY);
  if (open_rc < 0) {
    nk_vc_printf("Failed to open bar.txt\n");
  } else {
    nk_vc_printf("Successfully opened bar.txt\n");
    nk_vc_printf("File descriptor: %d\n", open_rc);
  }

  char buf[3];
  nk_vc_printf("Attempting to read first three chars of bar.txt\n");

  int bytes_read = read(open_rc, buf, 3);

  nk_vc_printf("Now attempting to write that to stdout\n");

  nk_vc_printf("Expecting:\n");
  nk_vc_printf("bar\n");
  nk_vc_printf("Received:\n");

  write(1, buf, 3);

  nk_vc_printf("\n");
}

void lseek_test() {
  nk_vc_printf("Testing lseek\n");
  nk_vc_printf("First, attempting to open bar.txt\n");
  int open_rc = open("/bar.txt", O_RDONLY);
  if (open_rc < 0) {
    nk_vc_printf("Failed to open bar.txt\n");
  } else {
    nk_vc_printf("Successfully opened bar.txt\n");
    nk_vc_printf("File descriptor: %d\n", open_rc);
  }

  char buf[3];
  nk_vc_printf("Reading the first 2 chars of bar.txt\n");
  int chars_read = read(open_rc, buf, 2);
  nk_vc_printf("This is what was read: ");
  nk_vc_printf(buf);
  nk_vc_printf("\n");

  nk_vc_printf("Now setting the seek position to be 1 further\n");
  int lseek_rc = lseek(open_rc, 1, 0);

  // Note on the return value:
  // A failed lseek returns (param #2) - 1
  // So in this case, if lseek fails, it will return 1-1=0
  if (!lseek_rc) {
    nk_vc_printf("lseek failed\n");
  } else {
    nk_vc_printf("lseek succeeded\n");
  }

  nk_vc_printf("Now, reading the first 2 chars after the new seek position\n");
  chars_read = read(open_rc, buf, 2);
  nk_vc_printf("This is what was read: ");
  nk_vc_printf(buf);
  nk_vc_printf("\n");
}

// fstat will not work until we make nk_fs_stat compatible with the c std stat
void fstat_test() {
  nk_vc_printf("Attempting to test fstat. I do not expect that this will work\n");
  nk_vc_printf("First, attempting to open foo.txt\n");
  int open_rc = open("/foo.txt", O_RDONLY);
  if (open_rc < 0) {
    nk_vc_printf("Failed to open foo.txt\n");
  } else {
    nk_vc_printf("Successfully opened foo.txt\n");
    nk_vc_printf("File descriptor: %d\n", open_rc);
  }

  nk_vc_printf("Now, attempting to do an fstat on foo\n");
  /* struct stat foo_stat; */
  /* int fstat_rc = fstat(open_rc, foo_stat); */
  /* if(fstat_rc) { */
  /*   nk_vc_printf("fstat failed\n"); */
  /* } else { */
  /*   nk_vc_printf("fstat succeeded\n"); */
  /* } */

  /* nk_vc_printf("Now, printing the contents of the stat struct:\n\n"); */
}
