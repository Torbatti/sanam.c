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
#include <dirent.h>
#include <fcntl.h>
#include <errno.h>

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

typedef unsigned int Io_File;

static int
file_open(Io_File *file, const char *path, int flags)
{
  int open_resault = open(path, flags);
  if(open_resault == -1)
  {
    return -1;
  }

  *file = (unsigned int)open_resault;

  return 0;
}

static int
file_readByte(Io_File *file)
{
  char buf[1];
  int  read_resault = read(*file, buf, 1);
  if(read_resault == -1)
  {
    printf("errno: %d", errno);
    return -1;
  }

  return buf[0];
}

// TODO(AABIB): should user buf[0] or buf[1] for a single char array?????
// static int
// file_readByte(Io_File *file, char buf[0])
// {
//   char buf[1];
//   int  read_resault = read(*file, buf, 1);
//   if(read_resault == -1)
//   {
//     printf("errno: %d", errno);
//     return -1;
//   }

//   return buf[0];
// }

static int
file_readBytes(Io_File *file, char *buf, int max_read)
{
  int read_resault = read(*file, buf, max_read);
  if(read_resault == -1)
  {
    printf("errno: %d", errno);
    return -1;
  }

  return read_resault;
}

static long
file_size(Io_File *file)
{
  struct stat file_stat;

  int fstat_resault = fstat(*file, &file_stat);
  if(fstat_resault == -1)
  {
    return -1;
  }

  return file_stat.st_size;
}

static int
file_close(Io_File *file)
{
  int close_resault = close(*file);
  if(close_resault == -1)
  {
    return -1;
  }

  *file = 0;
  return 0;
}

typedef DIR Io_Dir;

static int
dir_open(Io_Dir *dir, const char *path)
{
  dir = opendir(path);
  if(dir == NULL)
  {
    return -1;
  }
  return 0;
}

static int
dir_read(Io_Dir *dir, struct dirent *entry)
{
  entry = readdir(dir);
  if(entry == NULL)
  {
    return -1;
  }
  return 0;
}

static int
dir_close(Io_Dir *dir)
{
  int close_resault = closedir(dir);
  if(close_resault == -1)
  {
    return -1;
  }
  return 0;
}

typedef unsigned int utf8_Rune;

typedef struct utf8_Reader {
  unsigned int   index;
  unsigned char *buffer;
} utf8_Reader;

static int
utf8_readChar(utf8_Reader *reader, utf8_Rune *rune)
{
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
        *rune = 0;
        return -1;
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

int
main(int argc, char *argv[])
{}

/*
 * CHANGELOG:
 */
