/************************************************************************\
  Exemples de la formation
    "Ecriture de drivers et programmation noyau Linux"
  Chapitre "Ecriture de driver en mode caractere"

  (c) 2005-2015 Christophe Blaess
  http://www.blaess.fr/christophe/

\************************************************************************/

#ifndef EXEMPLE_06_H
#define EXEMPLE_06_H

	#include <asm/ioctl.h>

	#define EXEMPLE_IOCTL_MAGIC     'X' //eXample

	#define EXEMPLE_GET_PPID_FLAG  1
	#define EXEMPLE_SET_PPID_FLAG  2

	#define EXIOCGPPIDF _IOR(EXEMPLE_IOCTL_MAGIC, EXEMPLE_GET_PPID_FLAG, int) //Get
	#define EXIOCSPPIDF _IOW(EXEMPLE_IOCTL_MAGIC, EXEMPLE_SET_PPID_FLAG, int) //Set
        //           ^ Get/Set
        //      EXample
        //        IOContrl

#endif

