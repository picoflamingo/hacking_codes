/*
 * This file is part of lh_drop.
 * Copyright (c) David Martinez Oliveira 2016
 *
 * lh_drop is free software: you can redistribute
 * it and/or modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * lh_drop is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with lh_drop.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/* Simple example of privilege dropping on setuid processes */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int 
main ()
{
  uid_t  uid, euid;
  
  uid = getuid ();
  euid = geteuid();

  printf ("UID: %d EUID:%d\n", uid, euid);
  system ("/usr/bin/whoami");

  setuid (uid);
  system ("/usr/bin/whoami");

}

// make drop && sudo chown other drop && sudo chmod +s drop 
	
