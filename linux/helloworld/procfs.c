
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

/*
  procfs
  - linux/proc_fs.h
  seq_file: 简化了大内存的输出
  - linux/seq_file.h



  pr_err
  pr_info
 */


static struct proc_dir_entry *root;

int procfs_init (void) {
	if (!(root = proc_mkdir("helloworld", NULL))) {
		pr_err("Failed to create helloworld!");
		return -ENOMEM;
	}

	return 0;
}

void procfs_exit (void) {
	if (root)
		proc_remove(root);
}