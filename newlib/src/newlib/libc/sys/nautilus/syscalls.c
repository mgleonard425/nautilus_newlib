#include <sys/fcntl.h>
#include <sys/errno.h>

void
_exit(int status)
{
  printk("Thread called exit (status=%d)\n", status);
  nk_thread_exit((void*)(long)status);
}

int fork()
{
  // Nautilus has no notion of a process
  nk_vc_printf("Call to fork ignored\n");
  return -1;
}

int getpid()
{
  // Nautilus has no notion of a process
  nk_vc_printf("Call to getpid ignored\n");
  return -1;
}

int kill(int pid, int sig)
{
  // Nautilus has no notion of a process
  nk_vc_printf("Call to kill ignored\n");
  return -1;
}

int link(char *old, char *new)
{
  nk_vc_printf("Call to link ignored\n");
  return -1;
}

void * sbrk(int incr)
{
  // Idea: use Xiaoyang's implementation
  // How do I test this?
  printk("Call to sbrk ignored\n");
  return 0;
}

int unlink(char *name)
{
  nk_vc_printf("Call to unlink ignored\n");
  return -1;
}

int wait(int *status)
{
  // Nautilus has no notion of a process
  nk_vc_printf("Call to wait ignored\n");
  return -1;
}

int posix_memalign(void **memptr, size_t alignment, size_t size)
{
  nk_vc_printf("Call to vfork ignored\n");
  return -1;
}

int sigprocmask(int how, const sigset_t *sert, sigset_t *oldset)
{
  nk_vc_printf("Call to vfork ignored\n");
  return -1;
}

int _execve() {
  nk_vc_printf("Call to _execve ignored\n");
  return -1;
}

int execvpe() {
  // Nautilus has no notion of a process
  nk_vc_printf("Call to execvpe ignored\n");
  return -1;
}

int getdents() {
  nk_vc_printf("Call to getdents ignored\n");
  return -1;
}

int getgid() {
  // Nautilus has no notion of a process
  nk_vc_printf("Call to getgid ignored\n");
  return -1;
}

int getlogin() {
  // This probably won't work
  nk_vc_printf("Call to getlogin ignored\n");
  return -1;
}

int getpwnam() {
  // This also probably wont work
  nk_vc_printf("Call to getpwnam ignored\n");
  return -1;
}

int getpwuid() {
  // Likewise
  nk_vc_printf("Call to getpwuid ignored\n");
  return -1;
}

int getuid() {
  // Same
  nk_vc_printf("Call to getuid ignored\n");
  return -1;
}

int issetugid() {
  // Same here
  nk_vc_printf("Call to issetugid ignored\n");
  return -1;
}

int pipe() {
  // Nautilus has no notion of a process
  // Do we want to create a "pipe" between threads?
  nk_vc_printf("Call to pipe ignored\n");
  return -1;
}

int sched_setparam() {
  nk_vc_printf("Call to sched_setparam ignored\n");
  return -1;
}

int sched_setscheduler() {
  nk_vc_printf("Call to sched_setscheduler ignored\n");
  return -1;
}

int setegid() {
  // Is there a user in Nautilus?
  nk_vc_printf("Call to setegid ignored\n");
  return -1;
}

int seteuid() {
  // Is there a user in Nautilus?
  nk_vc_printf("Call to seteuid ignored\n");
  return -1;
}

int setpgid() {
  // Is there a user in Nautilus?
  nk_vc_printf("Call to setpgid ignored\n");
  return -1;
}

int sigaction() {
  nk_vc_printf("Call to sigaction ignored\n");
  return -1;
}

int vfork() {
  // Nautilus has no notion of a process
  nk_vc_printf("Call to vfork ignored\n");
  return -1;
}

int waitpid() {
  // Nautilus has no notion of a process
  nk_vc_printf("Call to waitpid ignored\n");
  return -1;
}
