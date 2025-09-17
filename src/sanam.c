/*
 * Copyright (c) 2025 Arya Bakhtiari
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/*
 * DOCUMENTATION:
 *
 *
 *
 *
 *
 * planned platforms:
 *   [ ] x86_64-windows
 *   [ ] x86_64-linux
 *   [ ] arm_64-linux
 *   [ ] x86_64-macos
 *   [ ] arm_64-macos
 *   [ ] x86_64-openbsd
 *   [ ] x86_64-netbsd
 *   [ ] x86_64-freebsd
 *
 */

char SANAM_VERSION[] = "UNTRACKED";

/*
 * Includes
 *
 *
 *
 */

#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

/*
 * Memmory related
 *
 */

/*
static int
mem_alloc(void *mem_pointer, unsigned int mem_size)
{

#ifdef _WIN32
  DWORD registerclass_err;

  mem_pointer = VirtualAlloc(0, mem_size, MEM_COMMIT, PAGE_READWRITE);

  if(!mem_pointer)
  {
    registerclass_err = GetLastError();
    return -1;
  }

  return 0;
#endif
}
*/

/*
static int
mem_clear()
{}
*/

/*
static int
mem_free(void *mem_pointer)
{
  VirtualFree(mem_pointer, 0, MEM_RELEASE);
  return 0;
}
*/

/*
 * File related
 *
 */

typedef struct Io_File {
  unsigned int fd;
  unsigned int pos;
} Io_File;

int
file_open(Io_File *file, const char *path, int flags)
{
  int open_resault = open(path, flags);
  if(open_resault == -1)
  {
    return -1;
  }

  file->fd  = (unsigned int)open_resault;
  file->pos = 0;

  return 0;
}

int
file_readByte(Io_File *file)
{
  char buf[1];
  int  read_resault = read(file->fd, buf, 1);
  if(read_resault == -1)
  {
    printf("errno: %d", errno);
    return -1;
  }

  file->pos = file->pos + read_resault;

  return buf[0];
}

int
file_readBytes(Io_File *file, char *buf, int max_read)
{
  int read_resault = read(file->fd, buf, max_read);
  if(read_resault == -1)
  {
    printf("errno: %d", errno);
    return -1;
  }

  file->pos = file->pos + read_resault;

  return read_resault;
}

long int
file_size(Io_File *file)
{
  struct stat file_stat;

  int fstat_resault = fstat(file->fd, &file_stat);
  if(fstat_resault == -1)
  {
    return -1;
  }

  return file_stat.st_size;
}

int
file_close(Io_File *file)
{
  int close_resault = close(file->fd);
  if(close_resault == -1)
  {
    return -1;
  }

  file->fd  = 0;
  file->pos = 0;
  return 0;
}

/*
 * Directory related
 *
 */

/*
static int
dir_open(DIR **ptrdir, const char *path)
{
 ptrdir = opendir(path);
 if(folder == NULL)
 {
   puts("Unable to read directory");
   return (1);
 }
 else
 {
   puts("Directory is opened!");
 }
}

static int
dir_close(DIR *ptrdir)
{
  closedir(dir);
}
*/

/*
 * Event loop
 *
 * backends: poll
 *
 *  NOTE(AABIB): have no idea how to implement it on windows ffs
 */

/*
 * UTF-8
 *
 *
 */

/*
 * NOTE(AABIB):
 */

typedef unsigned int utf8_Rune;

typedef struct utf8_Reader {
  unsigned int   index;
  unsigned char *buffer;
} utf8_Reader;

typedef struct {
  int       err;
  utf8_Rune rune;
} utf8_Rune_Error;

utf8_Rune_Error
utf8_readChar(utf8_Reader *reader)
{
  utf8_Rune_Error return_rune_err;
  return_rune_err.rune = 0;
  return_rune_err.err  = 0;

  unsigned char utf8_byte       = 0;
  unsigned int  utf8_rune       = 0;
  unsigned char utf8_rune_len   = 0;
  unsigned char utf8_rune_index = 0;

  while(1) /* NOTE(AABIB): CAN cause infinite loop */
  {
    utf8_byte = reader->buffer[reader->index];

    /*
    // if(utf8_byte == EOF)
    // {
    //   break;
    // }
    */

    if(utf8_rune_len == 0)
    {
      int temp_byte = utf8_byte;

      if((temp_byte & 128) == 0) /*0b10000000,0b00000000*/
      {
        utf8_rune_len = 1;
      }

      if((temp_byte & 224) == 192) /*0b11100000,0b11000000*/
      {
        utf8_rune_len = 2;
      }

      if((temp_byte & 240) == 224) /*0b11110000,0b11100000*/
      {
        utf8_rune_len = 3;
      }

      if((temp_byte & 248) == 240) /*0b11111000,0b11110000*/
      {
        utf8_rune_len = 4;
      }

      /* NOTE(AABIB): This shouldnt have happened */
      if(utf8_rune_len == 0)
      {
        return_rune_err.rune = 0;
        return_rune_err.err  = -1;
        return return_rune_err;
      }
    }

    utf8_rune = (utf8_rune << (8 * utf8_rune_index)) + utf8_byte;

    if(utf8_rune_len != utf8_rune_index)
    {
      utf8_rune_index = utf8_rune_index + 1;
    }

    if(utf8_rune_len == utf8_rune_index)
    {
      reader->index = reader->index + utf8_rune_len;

      /*
      //   DO WE NEED THIS ?
      //   utf8_rune       = 0;
      //   utf8_rune_len   = 0;
      //   utf8_rune_index = 0;
      */
    }
  }
}

/*
void
utf8_readLine()
{}

void
utf8_readDelimiter()
{}
*/

/*
 * Yaml parser
 * https://yaml.org/spec/1.2.2/
 *
 */

/*
 * CommonMark parser/renderer
 * https://spec.commonmark.org/0.31.2/
 *
 */

struct md_Token {
  unsigned int tag;
  unsigned int start_pos;
};

#include <stdio.h>

int
main(int argc, char *argv[])
{

}

/*
 * CHANGELOG:
 */
