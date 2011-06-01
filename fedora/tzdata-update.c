#define _GNU_SOURCE 1
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <syscall.h>
#include <unistd.h>

char buffer[32768], data[32768];
size_t datasize;
char zonename[1024];

ssize_t
readall (int fd, void *buf, size_t len)
{
  size_t n = len;
  ssize_t ret;
  do
    {
      ret = read (fd, buf, n);
      if (ret < 0)
	break;
      else if (ret == 0)
	break;
      buf = (char *) buf + ret;
      n -= ret;
    }
  while (n > 0);
  return ret < 0 ? ret : (ssize_t) (len - n);
}

ssize_t
writeall (int fd, const void *buf, size_t len)
{
  size_t n = len;
  ssize_t ret;
  do
    {
      ret = write (fd, buf, n);
      if (ret < 0)
	break;
      else if (ret == 0)
	break;
      buf = (const char *) buf + ret;
      n -= ret;
    }
  while (n > 0);
  return ret < 0 ? ret : (ssize_t) (len - n);
}

void
update (const char *filename)
{
  int fd = open (filename, O_RDONLY);
  if (fd < 0)
    return;
  ssize_t ret = readall (fd, buffer, sizeof (buffer));
  close (fd);
  if (ret <= 0 || (size_t) ret == sizeof (buffer))
    return;
  /* Don't update the file unnecessarily.  */
  if ((size_t) ret == datasize
      && memcmp (buffer, data, datasize) == 0)
    return;
  size_t len = strlen (filename);
  char tempfilename[len + sizeof (".tzupdate")];
  memcpy (tempfilename, filename, len);
  memcpy (tempfilename + len, ".tzupdate", sizeof (".tzupdate"));

  fd = open (tempfilename, O_WRONLY | O_CREAT | O_EXCL, 0600);
  if (fd < 0)
    return;
  if (writeall (fd, data, datasize) != datasize)
    {
clean_up:
      unlink (tempfilename);
      close (fd);
      return;
    }
  long int sret;
  sret = fchmod (fd, 0644);
  if (sret < 0)
    goto clean_up;
  close (fd);

  sret = rename (tempfilename, filename);
  if (sret < 0)
    unlink (tempfilename);
}

int
main (int argc, char **argv)
{
  int fd = open ("/etc/sysconfig/clock", O_RDONLY);
  if (fd < 0)
    return 0;
  ssize_t ret = readall (fd, buffer, sizeof (buffer) - 1);
  close (fd);
  if (ret <= 0 || (size_t) ret == sizeof (buffer) - 1)
    return 0;
  char *p = buffer;
  while (p != NULL)
    {
      while (*p == ' ' || *p == '\t') p++;
      if (memcmp (p, "ZONE", 4) == 0)
	{
	  p += 4;
	  while (*p == ' ' || *p == '\t') p++;
	  if (*p == '=')
	    {
	      p++;
	      while (*p == ' ' || *p == '\t') p++;
	      if (*p == '"') p++;
	      char *q = p;
	      while (strchr (" \t\n\"", *p) == NULL) p++;
	      const char path[] = "/usr/share/zoneinfo/";
	      if (p - q >= sizeof (zonename) - sizeof (path))
		return 0;
	      memcpy (zonename, path, sizeof (path) - 1);
	      memcpy (zonename + sizeof (path) - 1, q, p - q);
	      break;
	    }
	}
      p = strchr (p, '\n');
      if (p) p++;
    }
  if (*zonename == '\0')
    return 0;
  fd = open (zonename, O_RDONLY);
  if (fd < 0)
    return 0;
  ret = readall (fd, data, sizeof (data));
  close (fd);
  if (ret <= 0 || (size_t) ret == sizeof (data))
    return 0;
  datasize = (size_t) ret;
  update ("/etc/localtime");
  update ("/var/spool/postfix/etc/localtime");
  return 0;
}
