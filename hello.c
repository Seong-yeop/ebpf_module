#include <linux/module.h>    // included for all kernel modules
#include <linux/kernel.h>    // included for KERN_INFO
#include <linux/init.h>      // included for __init and __exit macros
#include <linux/unistd.h>
#include <linux/moduleparam.h>
#include <linux/version.h> 
#include <linux/kallsyms.h>

#include <linux/sched.h> 
#include <linux/uaccess.h> 


#include "linux_driver/include/libxdma_api.h"
#include "linux_driver/include/xocl_ioctl.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Lakshmanan");
MODULE_DESCRIPTION("A Simple Hello World module");

asmlinkage long (*sys_open_call) (const char *, int, int);
//asmlinkage long (*sys_read_call) (int, int, int);
asmlinkage long (*sys_ioctl_call) (unsigned int, unsigned int, void*); 

unsigned long *my_sys_call_table = NULL; 


static int __init hello_init(void)
{
	long fd;
	//struct drm_xocl_info *info;

	printk(KERN_INFO "Hello world!\n");

	if (!(my_sys_call_table = (void*)kallsyms_lookup_name("sys_call_table"))) {
	  printk(KERN_INFO "sys_call_table get failed!\n");
    return -1; 
  }
  printk(KERN_INFO "Syscall table loaded from %lx\n", (unsigned long) my_sys_call_table);


	/*
		 ret = request_module("xocl");
		 if (ret == 0)
		 printk(KERN_INFO "request module (xocl) loaded successfully\n");
		 else
		 printk(KERN_INFO "request module (xocl) failed to load\n");
		 */
	sys_open_call = (void*)my_sys_call_table[__NR_open];
  printk(KERN_INFO "sys_open loaded from %lx\n", (unsigned long) sys_open_call);
	sys_ioctl_call = (void*)my_sys_call_table[__NR_ioctl];
  printk(KERN_INFO "sys_ioctl loaded from %lx\n", (unsigned long) sys_ioctl_call);

	//fd = sys_open_call("/dev/dri/renderD128", O_RDWR, 0666);
	fd = file_("/dev/zero", O_RDONLY, 0);
  //vfs_read(fd, &tmp, 1, 0);

	printk(KERN_INFO "fd: %ld\n", fd);
	if (fd > 0)
		printk(KERN_INFO "open sucessfully\n");

	/* 
		 ret = sys_ioctl_call(fd, DRM_XOCL_INFO, info);
		 if (ret == 0) {
		 printk(KERN_INFO "ioctl success\n");
		 printk(KERN_INFO "device name: %d\n", info->device);
		 }
		 else {
		 printk(KERN_INFO "ioctl failed\n");
		 }
		 */
	return 0;    // Non-zero return means that the module couldn't be loaded.
}

static void __exit hello_cleanup(void)
{
	printk(KERN_INFO "Cleaning up module.\n");
}

module_init(hello_init);
module_exit(hello_cleanup);
