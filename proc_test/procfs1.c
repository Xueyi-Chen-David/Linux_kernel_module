/*
 *  procfs1.c -  create a "file" in /proc
 *
 */

#include <linux/module.h>	/* Specifically, a module */
#include <linux/kernel.h>	/* We're doing kernel work */
#include <linux/proc_fs.h>	/* Necessary because we use the proc fs */
#include <linux/uaccess.h>

#define procfs_name "helloworld"
#define BUFSIZE  100

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Chen H.Yi");
MODULE_DESCRIPTION("Yeah! This is my first write to /proc file, and the origin reference code is out of date, so I search a little bit and correct the code.");


struct proc_dir_entry *Our_Proc_File;


static ssize_t 
procfile_read(struct file *file, char __user *ubf, size_t count, loff_t *ppos)
{
	int len=0;
	char buffer[BUFSIZE];

	if(*ppos > 0 || count < BUFSIZE)
        return 0;
	
	printk(KERN_INFO "procfile_read (/proc/%s) called\n", procfs_name);

	len = sprintf(buffer, "HelloWorld!\n");

	if(copy_to_user(ubf,buffer,len))
        return -EFAULT;

	*ppos = len;

	return len;
}

static const struct file_operations proc_file_fops = {
	.owner = THIS_MODULE,
	.read = procfile_read
};

int init_module()
{
	Our_Proc_File = proc_create(procfs_name, 0644, NULL, &proc_file_fops);
	
	if (Our_Proc_File == NULL) {
		remove_proc_entry(procfs_name, NULL);
		printk(KERN_ALERT "Error: Could not initialize /proc/%s\n",
		       procfs_name);
		return -ENOMEM;
	}


	printk(KERN_INFO "/proc/%s created\n", procfs_name);	
	return 0;	/* everything is ok */
}

void cleanup_module()
{
	remove_proc_entry(procfs_name, NULL);
	printk(KERN_INFO "/proc/%s removed\n", procfs_name);
}
