/************************************************************************\
  Exemples de la formation
    "Ecriture de drivers et programmation noyau Linux"
  Chapitre "Programmer pour le noyau Linux"

  (c) 2005-2015 Christophe Blaess
  http://www.blaess.fr/christophe/

\************************************************************************/

	#include <linux/module.h>
	#include <linux/proc_fs.h>
	#include <linux/version.h>


	static const struct file_operations exemple_fops = {
		.owner=THIS_MODULE, //owner is the driver
	};


static int __init exemple_init (void)
{
	struct proc_dir_entry * entry;

	entry = proc_create(THIS_MODULE->name, S_IFREG | 0644, NULL, &exemple_fops);// REGular InterFace, NULL for /proc
	if (entry == NULL)
		return -EBUSY;

	return 0;
}


static void __exit exemple_exit (void)
{
	remove_proc_entry(THIS_MODULE->name, NULL);
}


	module_init(exemple_init);
	module_exit(exemple_exit);

	MODULE_DESCRIPTION("/proc entry creation.");
	MODULE_AUTHOR("Christophe Blaess <Christophe.Blaess@Logilin.fr>");
	MODULE_LICENSE("GPL");

