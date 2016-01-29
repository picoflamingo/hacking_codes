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

#ifndef SSECTION_H
#define SSECTION_H


#include <elf.h>

#define SECURE_FUNC __attribute__((section(".secure")))

/* Proof Of Concept. Just a fixed key in here */
#define XKEY 0x1f

#ifdef __cplusplus
extern "C" {
#endif

  int          get_file_size (int fd);
  void         dump_elf_info (void *data);
  void         dump_mem (unsigned char *ptr, unsigned char *ptr1);

  int          open_file (char *fname);
  void*        map_elf (int fd);
  Elf64_Shdr*  find_section (void *data, char *name);

  void         xor_section (void *data, Elf64_Shdr *sec);
  void         xor_mem (unsigned char *text, int size);
  void         crypt (char *fname);
  void         decrypt (char *fname);
  void         decrypt_mem (unsigned char *ptr, unsigned char *ptr1);

#ifdef __cplusplus
}
#endif

#endif
