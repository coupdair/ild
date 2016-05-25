/************************************************************************\
  Exemples de la formation
    "Ecriture de drivers et programmation noyau Linux"
  Chapitre "Ecriture de driver en mode caractere"

  (c) 2005-2015 Christophe Blaess
  http://www.blaess.fr/christophe/

\************************************************************************/

	#include <linux/completion.h>
	#include <linux/delay.h>
	#include <linux/fs.h>
	#include <linux/kthread.h>
	#include <linux/module.h>

	static DECLARE_COMPLETION(exemple_started);
	static DECLARE_COMPLETION(exemple_stopped);

	static int exemple_stop = 0;


int exemple_thread(void * arg)
{
	complete(&exemple_started);//1. wait 

	while (! exemple_stop) {
		printk(KERN_INFO "%s - %s(): Thread running, jiffies = %lu\n",
		                 THIS_MODULE->name, __FUNCTION__, jiffies);
		ssleep(1);
	}
	complete_and_exit(&exemple_stopped, 0);//oo. return 
}


static int __init exemple_init (void)
{
	struct task_struct * thread;

	thread = kthread_run(exemple_thread, NULL, THIS_MODULE->name);
	if (IS_ERR(thread))
              return -ENOMEM;
        wait_for_completion(&exemple_started);//0. wait 
	printk(KERN_INFO "%s - %s(): Thread started\n", THIS_MODULE->name, __FUNCTION__);

	return 0; 
}


static void __exit exemple_exit (void)
{
	exemple_stop = 1;
	wait_for_completion(&exemple_stopped);//oo. wait 

	printk(KERN_INFO "%s - %s(): Thread terminated\n", THIS_MODULE->name, __FUNCTION__);
}


	module_init(exemple_init);
	module_exit(exemple_exit);

	MODULE_DESCRIPTION("Kernel thread implementation.");
	MODULE_AUTHOR("Christophe Blaess <Christophe.Blaess@Logilin.fr>");
	MODULE_LICENSE("GPL");

