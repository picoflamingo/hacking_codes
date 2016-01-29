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
#include <string.h>
#include "pocrypt.h"


SECURE_FUNC int
my_secure_function (char *str)
{
  printf ("You said: %s\n", str);
  return 0;
}

/* End of secure region mark */
SECURE_FUNC void dummy (){};

int 
main (int argc, char *argv[])
{
  printf ("Hello. I'm a target\n");
  if (argc != 2)
    {
      fprintf (stderr, "I need a key\n");
      return 1;
    }
#ifdef DUMP
  dump_mem ((unsigned char*)&my_secure_function, (unsigned char*)&dummy);
#endif

  set_key (argv[1]);
  decrypt_mem ((unsigned char*)&my_secure_function, (unsigned char*)&dummy);

#ifdef DUMP
  dump_mem ((unsigned char*)&my_secure_function, (unsigned char*)&dummy);
#endif

  my_secure_function ("Bye!\n");

  return 0;
}
 
