/*
 * This file is part of PoCrypt.
 * Copyright (c) David Martinez Oliveira
 *
 * PoCrypt is free software: you can redistribute
 * it and/or modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * PoCrypt is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with PoCrypt.  If not, see <http://www.gnu.org/licenses/>.
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>


#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#include <elf.h>
#include <sys/mman.h>

#include "pocrypt.h"

// Just one key to work with
static char *key = NULL;
static int key_len = 0;


int 
get_file_size (int fd)
{
  struct stat _info;
  
  fstat (fd, &_info);

  return _info.st_size;
}

void 
dump_mem (unsigned char *ptr, unsigned char *ptr1)
{
  int i = 0;
  
  printf ("-- DUMP: %p - %p ---------------------------\n", ptr, ptr1);
  while (ptr < ptr1)
    {
      i++;
      printf ("%02x%s", *ptr, !(i%16)? "\n": " ");

      ptr++;
    }
  printf ("\n---------------------------------------------\n");
}


/* XXX: TO be extended to dump some more information */
void
dump_elf_info (void *data)
{
  /*XXX: This code only works on 64bits ELF*/
  Elf64_Ehdr* hdr = (Elf64_Ehdr *) data; 
  
  printf ("Entry Point: 0x%0x\n", (int) hdr->e_entry);
  
}


int
open_file (char *fname)
{
  int fd;
  
  if ((fd = open (fname, O_APPEND | O_RDWR, 0)) < 0)
    {
      perror ("open:");
      exit (1);
    }

  printf ("+ File '%s' open with fd: %d\n", fname, fd);

  return fd;  
}

void*
map_elf (int fd)
{
  void *data;
  int  size;
  
  size = get_file_size (fd);
  if ((data = mmap (0, size, PROT_READ| PROT_WRITE| PROT_EXEC,
		    MAP_SHARED, fd, 0)) == MAP_FAILED)
    {
      perror ("mmap:");
      exit (1);
    }
  
  printf ("+ File mapped at %p\n", data);
  
  return data;
}

Elf64_Shdr *
find_section (void *data, char *name)
{
  char        *sname;
  int         i;
  Elf64_Ehdr* elf_hdr = (Elf64_Ehdr *) data;
  Elf64_Shdr *shdr = (Elf64_Shdr *)(data + elf_hdr->e_shoff);
  Elf64_Shdr *sh_strtab = &shdr[elf_hdr->e_shstrndx];
  const char *const sh_strtab_p = data + sh_strtab->sh_offset;
   
  printf ("+ %d section in file. Looking for section '%s'\n", 
	  elf_hdr->e_shnum, name);
  
  
  for (i = 0; i < elf_hdr->e_shnum; i++)
    {
      sname = (char*) (sh_strtab_p + shdr[i].sh_name);
      if (!strcmp (sname, name))  return &shdr[i];
      
    }
  
  return NULL;
}

void 
xor_mem (unsigned char *text, int size)
{
  int i;

  printf ("+ Decoding %d bytes at %p\n", size, text);
  for (i = 0; i < size; i++) *(text+i) ^= *(key + (i % key_len));

}


void 
xor_section (void *data, Elf64_Shdr *sec)
{
  unsigned char *text = (unsigned char*)data;
  int offset = sec->sh_offset;
  int size = sec->sh_size;
  int i;
  
  text += offset;


  printf ("+ XORing %d bytes at %p\n", size, text);

#ifdef DUMP
  printf ("ORIGINAL:\n");
  dump_mem (text, text+size);
#endif

  xor_mem (text, size);

#ifdef DUMP
  printf ("XORed:\n");
  dump_mem (text, text+size);
#endif

}



void 
crypt (char *fname)
{
  int fd;
  void *data;
  Elf64_Shdr* sec;

  if (!key)
    {
      fprintf (stderr, "No key set...Aborting\n");
      exit (1);
    }

  fd = open_file (fname);

  data = map_elf (fd);
  sec = find_section (data, ".secure");

  xor_section (data, sec);

  close (fd);
  exit (1);
}

void
decrypt_mem (unsigned char *ptr, unsigned char *ptr1)
{
  /* Calculate page boundary for the provided src pointer*/
  size_t pagesize = sysconf(_SC_PAGESIZE);
  uintptr_t pagestart = (uintptr_t)ptr & -pagesize;

  /* Caculate size of memory block w.r.t pagestart */
  int psize = (((ptr1 - (unsigned char*)pagestart)));

  if (!key)
    {
      fprintf (stderr, "No key set...Aborting\n");
      exit (1);
    }

  printf ("+ Decoding %p -> %p\n", ptr, ptr1);
  printf ("+ Changing permissions 0x%0x (0x%x)\n", (int)pagestart, (int)psize);

  /* Make the pages writable...*/
  if (mprotect ((void*)pagestart, psize, PROT_READ | PROT_WRITE | PROT_EXEC) < 0)
    perror ("mprotect:");
  
  xor_mem (ptr, ptr1 - ptr);
  
}


void
set_key (char *str)
{
  if (!key) free (key);

  key = strdup (str);
  key_len = strlen (str);
}

