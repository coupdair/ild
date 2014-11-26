/************************************************************************\
  exemple_14 - Chapitre "Programmer pour le noyau Linux"

  Callback d'ecriture dans /proc

  Exemples de la formation "Programmation Noyau sous Linux"

  (c) 2005-2014 Christophe Blaess
  http://www.blaess.fr/christophe/

\************************************************************************/

#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/version.h>

#include <asm/uaccess.h>

static char * nom_entree = "exemple_14";

static int valeur_exemple = 0;

static ssize_t lecture  (struct file *, char __user *, size_t, loff_t *);
static ssize_t ecriture (struct file *, const char __user *, size_t, loff_t *);

static const struct file_operations exemple_14_proc_fops = {
	.owner	= THIS_MODULE,
	.read   = lecture,
	.write  = ecriture,
};


static int __init exemple_14_init (void)
{
	struct proc_dir_entry * entree;
	entree = proc_create(nom_entree, S_IFREG | 0644, NULL, & exemple_14_proc_fops);
	if (entree == NULL)
		return -EBUSY;
	return 0; 
}


static void __exit exemple_14_exit (void)
{
	remove_proc_entry(nom_entree, NULL);
}


static ssize_t lecture (struct file * filp, char __user * u_buffer, size_t max, loff_t * offset)
{
	char buffer[128];
	int  nb;

	snprintf(buffer, 128, "PID=%u, PPID=%u, Nom=%s, valeur=%d\n",
	         current->pid, 
	         current->real_parent->pid,
	         current->comm,
	         valeur_exemple);

	nb = strlen(buffer) - (*offset);
	if (nb <= 0)
		return 0;
	if (nb > max)
		nb = max;
	if (copy_to_user(u_buffer, & (buffer[*offset]), nb) != 0)
		return -EFAULT;
	(*offset) += nb;
	return nb;
}


static ssize_t ecriture (struct file * filp, const char __user * u_buffer, size_t nb, loff_t * unused)
{
	char buffer[128];
	if (nb >= 128)
		return -ENOMEM;
	if (copy_from_user(buffer, u_buffer, nb) != 0)
		return -EFAULT;
	if (sscanf(buffer, "%d", & valeur_exemple) != 1)
		return -EINVAL;
	return nb;
}

module_init(exemple_14_init);
module_exit(exemple_14_exit);
MODULE_LICENSE("GPL");


