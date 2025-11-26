#include <linux/init.h>
#include <linux/module.h>

static int kmod_init (void) {
	printk(KERN_ALERT "load hello world kernel module\n");
	return 0;
}

static void kmod_exit (void) {
	printk(KERN_ALERT "remove hello world kernel module\n");
}

module_init(kmod_init);
module_exit(kmod_exit);

MODULE_AUTHOR("Wenger Binning");
MODULE_DESCRIPTION("This is a demo Kernel module");
MODULE_VERSION("0.0.0.1");
MODULE_LICENSE("GPL");
