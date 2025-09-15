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
 * Os specific Includes
 *
 *
 *
 */

#ifdef _WIN32 /* Both 32 bit and 64 bit */

#  include <Windows.h>
#  include <stdio.h>

#endif /* _WIN32 */

#ifdef __linux__
#endif /* __linux__ */

#ifdef __MACH__
#endif /* __MACH__ */

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

static int
file_open(FILE **ptrfile, const char *name, const char *mode)
{
  *ptrfile = fopen(name, mode);
  if(ptrfile == NULL)
  {
    return -1;
  }
  return 0;
}

/* NOTE(AABIB): better api interface i guess? */
static int
file_readByte(FILE *ptrfile)
{
  int fgetc_resault = fgetc(ptrfile);
  return fgetc_resault;
}

/* TODO(AABIB): Better errors handling */
static long
file_readBytes(FILE *ptrfile, char *ptrbuff, int max_read)
{
  long startpos = ftell(ptrfile);
  if(startpos == -1)
  {
    return -1;
  }

  char *fgets_resault = fgets(ptrbuff, max_read, ptrfile);
  /* TODO(AABIB): Handle errors */
  /* fgets_resault == null and errno is set */
  /*
  // if(!fgets_resault)
  // {
  //   return -1;
  // }
  */

  long endpos = ftell(ptrfile);
  if(endpos == -1)
  {
    return -1;
  }

  /* + 1 is not needed , endpos hasnt been read yet*/
  long read_len = endpos - startpos;
  return read_len;
}

/*
static int
file_read()
{}
*/

static int
file_close(FILE *ptrfile)
{
  int fclose_resault = fclose(ptrfile);

  /* returns 0 on success*/
  if(fclose_resault == 0)
  {
    return 0;
  }
  return -1;
}

/*
 * Directory related
 *
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
  char buff[2048];

  FILE *file        = NULL;
  char  file_name[] = "test.md";
  char  file_mode[] = "r";

  int fileopen_resault = file_open(&file, "test.md", file_mode);

  if(fileopen_resault == -1)
  {
    printf("Unable to open file %s\n", file_name);
    return -1;
  }

  while(1)
  {
    char *fgets_resault = fgets(buff, 2048, file);
    if(fgets_resault == NULL)
    {
      break;
    }
    printf("%s", buff);
  }

  int fileclose_resault = file_close(file);
  if(fileopen_resault == -1)
  {
    printf("Unable to close file %s\n", file_name);
    return -1;
  }

  return 0;
}

/*
 * CHANGELOG:
 */
