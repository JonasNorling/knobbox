/**
 * Dummmy implementations of system calls from the newlib C library,
 * needed for linking.
 */
#include <signal.h>
#include <unistd.h>
#include <sys/stat.h>

#define UU __attribute__((unused))

void _exit(UU int status)
{
  while (1);
}

int _kill(UU pid_t pid,
	  UU int sig)
{
  return 0;
}

int _getpid()
{
  return 1;
}

void* _sbrk(UU int increment)
{
  return 0;
}

ssize_t _write(UU int fd, UU const void *buf, UU size_t count)
{
  return 0;
}

int _close(UU int fd)
{
  return 0;
}

int _fstat(UU int fd, UU struct stat *buf)
{
  return 0;
}

int _isatty(UU int fd)
{
  return 0;
}

off_t _lseek(UU int fd, UU off_t offset, UU int whence)
{
  return 0;
}

ssize_t _read(UU int fd, UU void *buf, UU size_t count)
{
  return 0;
}

