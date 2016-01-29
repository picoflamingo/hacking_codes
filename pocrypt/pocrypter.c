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

#include "pocrypt.h"


int
main (int argc, char *argv[])
{
  printf ("PoCrypt. Proof Of Concept Crypter\n");
  if (argc < 1)
    {
      fprintf (stderr, "Invalid Number of parameters\n");
      fprintf (stderr, "%s key file\n", argv[0]);
      exit (1);
    }

  set_key (argv[1]);
  crypt (argv[2]);

  return 0;
}
