/************************************************************************\
  Exemples de la formation
    "Ecriture de drivers et programmation noyau Linux"
  Chapitre "Programmer pour le noyau Linux"

  (c) 2005-2015 Christophe Blaess
  http://www.blaess.fr/christophe/

\************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>


void main(void)
{
  #define SIZE 64
	struct timeval  tv_tod[SIZE];
  int i;
  for(i=0;i<SIZE;i++)
  {
    gettimeofday(&tv_tod[i], NULL);
//clock_gettime(CLOCK_REALTIME
  }//i loop
  for(i=0;i<SIZE;i++)
  {
	printf("%04d: tv_tod.tv_sec = %ld, tv_tod.tv_usec = %ld\n",
	                  i, tv_tod[i].tv_sec, tv_tod[i].tv_usec);
  }//i loop

}

