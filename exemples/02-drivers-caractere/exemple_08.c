/************************************************************************\
  Exemples de la formation
    "Ecriture de drivers et programmation noyau Linux"
  Chapitre "Ecriture de driver en mode caractere"

  (c) 2005-2015 Christophe Blaess
  http://www.blaess.fr/christophe/

\************************************************************************/

	#include <linux/cdev.h>
	#include <linux/delay.h>
	#include <linux/device.h>
	#include <linux/fs.h>
	#include <linux/miscdevice.h>
	#include <linux/module.h>
	#include <linux/mutex.h>
	#include <linux/sched.h>

	#include <asm/uaccess.h>


	static ssize_t exemple_read  (struct file * filp, char * buffer,
	                              size_t length, loff_t * offset);
	static ssize_t example_write (struct file * filp, const char * buffer,
	                              size_t length, loff_t * offset);

	static struct file_operations fops_exemple = {
		.owner   =  THIS_MODULE,
		.read    =  exemple_read,
		.write   =  example_write,
	};

	static struct miscdevice exemple_misc_driver = {
		    .minor          = MISC_DYNAMIC_MINOR,
		    .name           = THIS_MODULE->name,
		    .fops           = & fops_exemple,
	};

	static char message[32];
	static int  size=0;
	DEFINE_MUTEX(mutex_message);


static int __init exemple_init (void)
{
  strcpy(message,"driver\n");
  size=strlen(message);
	return misc_register(& exemple_misc_driver);
}


static void __exit exemple_exit (void)
{
	misc_deregister(& exemple_misc_driver);
}


static ssize_t exemple_read(struct file * filp, char * buffer,
                            size_t length, loff_t * offset)
{
  int lg;
  if (mutex_lock_interruptible(&mutex_message)!=0)
  {
    printk(KERN_INFO "%s - %s() interrupted (i.e. signal received)\n", THIS_MODULE->name, __FUNCTION__);
    return -ERESTARTSYS;//return straightforward to treat an other signal
  }
  size=strlen(message);
  if (length < size)
  {
    mutex_unlock(&mutex_message);
    return -ENOMEM;
  }
  lg=size-(*offset);
  if(lg<=0)
  {
    mutex_unlock(&mutex_message);
    return 0;//entire message readed
  }
  if (length < lg)
    lg = length;
  if (copy_to_user(buffer, &message[*offset], lg) != 0)
  {
    mutex_unlock(&mutex_message);
    return -EFAULT;
  }
  mutex_unlock(&mutex_message);
  *offset+=lg;
  return lg;
}

static ssize_t example_write(struct file * filp, const char * buffer,
                             size_t length, loff_t * offset)
{
  if (mutex_lock_interruptible(&mutex_message)!=0)
  {
    printk(KERN_INFO "%s - %s() interrupted (i.e. signal received)\n", THIS_MODULE->name, __FUNCTION__);
    return -ERESTARTSYS;//return straightforward to treat an other signal
  }
  size=strlen(message);
  if (length < size)
  {
    mutex_unlock(&mutex_message);
    return -ENOMEM;
  }
  if (copy_from_user(message, buffer, size) != 0)
  {
    mutex_unlock(&mutex_message);
    return -EFAULT;
  }
  mutex_unlock(&mutex_message);
  return size;
}

	module_init(exemple_init);
	module_exit(exemple_exit);

	MODULE_DESCRIPTION("Use of a mutex for shared variable protection (message), read and write a message, i.e. mail box.");
	MODULE_AUTHOR("Christophe Blaess <Christophe.Blaess@Logilin.fr>");
	MODULE_LICENSE("GPL");

