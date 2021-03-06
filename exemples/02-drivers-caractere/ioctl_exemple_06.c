/************************************************************************\
  Exemples de la formation
    "Ecriture de drivers et programmation noyau Linux"
  Chapitre "Ecriture de driver en mode caractere"

  (c) 2005-2015 Christophe Blaess
  http://www.blaess.fr/christophe/

\************************************************************************/

	#include <stdio.h>
	#include <stdlib.h>
	#include <unistd.h>
	#include <fcntl.h>
	#include <sys/ioctl.h>

	#include "exemple_06.h"


int main(int argc, char * argv[])
{
	int fd;
	int ppid_flag;

	if (argc < 2) {
		fprintf(stderr, "usage: %s filename [ppid_flag]\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	fd = open(argv[1], O_RDONLY, 0);
	if (fd < 0) {
		perror(argv[1]);
		exit(EXIT_FAILURE);
	}

	if (argc > 2)
		if (sscanf(argv[2], "%d", & ppid_flag) == 1)
			if (ioctl(fd, EXIOCSPPIDF, &ppid_flag) != 0)
				perror("ioctl(EXIOCSPPIDF)");

	if (ioctl(fd, EXIOCGPPIDF, &ppid_flag) != 0) {
		perror("ioctl(EXIOCGPPIDF)");
		exit(EXIT_FAILURE);
	}
	fprintf(stdout, "%d\n", ppid_flag);
	return 0;
}

