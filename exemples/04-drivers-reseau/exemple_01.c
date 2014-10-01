/************************************************************************\
  Chapitre "Peripheriques et protocoles reseau"
  exemple_01

  Affectation d'adresse materielle

  Exemples de la formation "Programmation Noyau sous Linux"

  (c) 2005-2014 Christophe Blaess
  http://www.blaess.fr/christophe/

\************************************************************************/

#include <linux/module.h>
#include <linux/version.h>

#include <linux/netdevice.h>
#include <linux/etherdevice.h>

	struct net_device * net_dev_ex = NULL;

static int exemple_01_open (struct net_device * net_dev)
{
	printk(KERN_INFO "%s: exemple_01_open(%p)\n",
	       THIS_MODULE->name, net_dev);
	// Remplir l'adresse physique en interrogeant le materiel.
	// Ici on la simule avec des valeurs arbitraires.
	net_dev->dev_addr[0] = 0x00;
	net_dev->dev_addr[1] = 0x12;
	net_dev->dev_addr[2] = 0x34;
	net_dev->dev_addr[3] = 0x56;
	net_dev->dev_addr[4] = 0x78;
	net_dev->dev_addr[5] = 0x00;

	// Indiquer que l'interface peut traiter des paquets
	netif_start_queue(net_dev);

	return 0;
}


static int exemple_01_stop (struct net_device * net_dev)
{
	printk(KERN_INFO "%s: exemple_01_stop(%p)\n",
	       THIS_MODULE->name, net_dev);

	// L'interface ne peut plus traiter de paquets
	netif_stop_queue(net_dev);

	return 0;
}


static int exemple_01_start_xmit(struct sk_buff * sk_b, struct net_device * src)
{
	printk(KERN_INFO "%s: exemple_01_start_xmit(%p,%p)\n",
	       THIS_MODULE->name, sk_b, src);

	// Acquitter les donnees transmises et quitter
	dev_kfree_skb(sk_b);
	return NETDEV_TX_OK;
}

struct net_device_ops exemple_01_netdev_ops = {
	.ndo_open       = exemple_01_open,
	.ndo_stop       = exemple_01_stop,
	.ndo_start_xmit = exemple_01_start_xmit,
};


static void exemple_01_setup (struct net_device * net_dev)
{
	printk(KERN_INFO "%s: exemple_01_setup(%p)\n",
	       THIS_MODULE->name, net_dev);

	// Objet de base : ethernet
	ether_setup(net_dev);

	// Surcharge de certaines methodes
	net_dev->netdev_ops = & exemple_01_netdev_ops;
}


static void exemple_01_exit(void);

static int __init exemple_01_init(void)
{
	
	printk(KERN_INFO "%s: exemple_01_init()\n", THIS_MODULE->name);

	net_dev_ex = alloc_netdev(0,"ex%d", exemple_01_setup);
	if (net_dev_ex == NULL)
		return -ENOMEM;

	if (register_netdev(net_dev_ex) != 0) {
		exemple_01_exit();
		return -ENODEV;
	}
	return 0;
}


static void exemple_01_exit(void)
{
	printk(KERN_INFO "%s: exemple_01_exit()\n", THIS_MODULE->name);

	if (net_dev_ex != NULL) {
		unregister_netdev(net_dev_ex);
		free_netdev(net_dev_ex);
		net_dev_ex = NULL;
	}
}

module_init(exemple_01_init)
module_exit(exemple_01_exit)
MODULE_LICENSE("GPL");