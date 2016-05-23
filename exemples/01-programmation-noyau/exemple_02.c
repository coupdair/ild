/************************************************************************\
  Exemples de la formation
    "Ecriture de drivers et programmation noyau Linux"
  Chapitre "Programmer pour le noyau Linux"

  (c) 2005-2015 Christophe Blaess
  http://www.blaess.fr/christophe/

\************************************************************************/

	#include <linux/module.h>

	static int input=0;
	module_param(input, int, 0644);//,,acces mod
	MODULE_PARM_DESC(input, "The input channel number.");//for modinfo


	static char * example_label = THIS_MODULE->name;
	module_param_named(label, example_label, charp, 0644);//name=label
	MODULE_PARM_DESC(label, "The label to use during communication.");


	static int param_table[8];
	static int nb_items;
	module_param_array(param_table, int, &nb_items, 0644);


static int __init exemple_init (void)
{
	int i;

	printk(KERN_INFO "%s: input = %d\n", THIS_MODULE->name, input);
	printk(KERN_INFO "%s: example_label = %s\n", THIS_MODULE->name, example_label);
	printk(KERN_INFO "%s: nb_items = %d\n", THIS_MODULE->name, nb_items);
	printk(KERN_INFO "%s: Table = ", THIS_MODULE->name);
	for (i = 0; i < nb_items; i ++)
		printk("%d ", param_table[i]);
	printk("\n");

	return 0;
}



static void __exit exemple_exit (void)
{
}


	module_init(exemple_init);
	module_exit(exemple_exit);

	MODULE_DESCRIPTION("Parameters on the insmod command line");
	MODULE_AUTHOR("Christophe Blaess <Christophe.Blaess@Logilin.fr>");
	MODULE_LICENSE("GPL");

