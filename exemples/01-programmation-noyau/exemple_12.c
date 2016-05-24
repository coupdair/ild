/************************************************************************\
  Exemples de la formation
    "Ecriture de drivers et programmation noyau Linux"
  Chapitre "Programmer pour le noyau Linux"

  (c) 2005-2015 Christophe Blaess
  http://www.blaess.fr/christophe/

\************************************************************************/

	#include <linux/module.h>
	#include <linux/proc_fs.h>
	#include <linux/sched.h>
	#include <linux/version.h>

	#include <asm/uaccess.h>
	#include <linux/slab.h>


	static ssize_t exemple_read (struct file *, char __user *, size_t, loff_t *);
	static int exemple_open (struct inode *, struct file *);
	static int exemple_release (struct inode *, struct file *);

	static const struct file_operations exemple_fops = {
		.owner	= THIS_MODULE,
		.read   = exemple_read,
		.open   = exemple_open,
		.release   = exemple_release,
	};


static int __init exemple_init (void)
{
	struct proc_dir_entry * entry;

	entry = proc_create(THIS_MODULE->name, S_IFREG | 0644, NULL, & exemple_fops);
	if (entry == NULL)
		return -EBUSY;

	return 0; 
}


static void __exit exemple_exit (void)
{
	remove_proc_entry(THIS_MODULE->name, NULL);
}


static ssize_t exemple_read (struct file * filp, char __user * u_buffer, size_t max, loff_t * offset)
{
	int  nb;

	printk(KERN_INFO "%s - %s(%p, %p, %u, %lld)", THIS_MODULE->name, __FUNCTION__, filp, u_buffer, (unsigned int) max, *offset);

	nb = strlen(filp->private_data)-(*offset);//buffer-allready_readed = next_to_read sizes
	if (nb <= 0) {
		printk(" -> 0\n");
		return 0;
	}

	if (nb > max)
		nb = max;

	if (copy_to_user(u_buffer, &(((char*)(filp->private_data))[*offset]), nb) != 0) {
		printk(" -> error\n");
		return -EFAULT;
	}
	(*offset) += nb;
	printk(" -> %d\n", nb);
	return nb;
}

static int exemple_open (struct inode *ind, struct file * filp)
{
  printk(KERN_INFO "%s - %s\n", THIS_MODULE->name, __FUNCTION__);
  filp->private_data=kmalloc(128, GFP_KERNEL);
  if(filp->private_data==NULL)
	return -ENOMEM;
	snprintf(filp->private_data, 128, "PID=%u, PPID=%u, Name=%s\n",
	         current->pid, 
	         current->real_parent->pid,
	         current->comm);
  return 0;
}
static int exemple_release (struct inode *ind, struct file * filp)
{
  printk(KERN_INFO "%s - %s\n", THIS_MODULE->name, __FUNCTION__);
  kfree(filp->private_data);
  return 0;
}

	module_init(exemple_init);
	module_exit(exemple_exit);

	MODULE_DESCRIPTION("/proc entry read callback improvement.");
	MODULE_AUTHOR("Christophe Blaess <Christophe.Blaess@Logilin.fr>");
	MODULE_LICENSE("GPL");
