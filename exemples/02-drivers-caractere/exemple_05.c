/************************************************************************\
  Exemples de la formation
    "Ecriture de drivers et programmation noyau Linux"
  Chapitre "Ecriture de driver en mode caractere"

  (c) 2005-2015 Christophe Blaess
  http://www.blaess.fr/christophe/

\************************************************************************/

	#include <linux/cdev.h>
	#include <linux/device.h>
	#include <linux/fs.h>
	#include <linux/miscdevice.h>
	#include <linux/module.h>
	#include <linux/sched.h>

	#include <asm/uaccess.h>

  //not safe, need to add mutex for access to 'compteur'
  int compteur=-1;

	static int     example_open  (struct inode * ind, struct file * filp);
	static ssize_t example_read  (struct file * filp, char * buffer,
	                              size_t length, loff_t * offset);
	static ssize_t example_write (struct file * filp, const char * buffer,
	                              size_t length, loff_t * offset);


	static struct file_operations fops_exemple = {
		.owner   =  THIS_MODULE,
		.read    =  example_read,
                .open    =  example_open,
		.write   =  example_write,
	};


	static struct miscdevice exemple_misc_driver = {
		    .minor          = MISC_DYNAMIC_MINOR ,
		    .name           = THIS_MODULE->name,
		    .fops           = & fops_exemple,
	};


static int __init exemple_init (void)
{
	return misc_register(& exemple_misc_driver);
}


static void __exit exemple_exit (void)
{
	misc_deregister(& exemple_misc_driver);
}

static int example_open(struct inode * ind, struct file * filp)
{
	printk(KERN_INFO "%s - %s()\n", THIS_MODULE->name, __FUNCTION__);
  ++compteur;
	return 0;
}

static ssize_t example_read(struct file * filp, char * buffer,
                            size_t length, loff_t * offset)
{
	char chaine[128];
	int lg;

	snprintf(chaine, 128, "compteur=%u\n",compteur);

	lg = strlen(chaine) - (*offset);

	if (lg <= 0)
		return 0;

	if (length < lg)
		lg = length;

	if (copy_to_user(buffer, & chaine[* offset], lg) != 0)
		return -EFAULT;

	*offset += lg;

	return lg;
}
static ssize_t example_write(struct file * filp, const char * buffer,
                             size_t length, loff_t * offset)
{
	char k_buffer[80];
	int val;

	if (length > 79)
		return -EINVAL;
	if (copy_from_user(k_buffer, buffer, length) != 0)
		return -EFAULT;
	if (sscanf(k_buffer, "%d", & val) != 1)
		return -EINVAL;

  compteur=--val;

	return length;
}


	module_init(exemple_init);
	module_exit(exemple_exit);

	MODULE_DESCRIPTION("read() system call implementation.");
	MODULE_AUTHOR("Christophe Blaess <Christophe.Blaess@Logilin.fr>");
	MODULE_LICENSE("GPL");

