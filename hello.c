#include <linux/module.h>    // included for all kernel modules
#include <linux/kernel.h>    // included for KERN_INFO
#include <linux/init.h>      // included for __init and __exit macros
#include <linux/unistd.h>
#include <linux/moduleparam.h>
#include <linux/version.h> 
#include <linux/kallsyms.h>
#include <linux/sched.h> 
#include <linux/uaccess.h> 
#include <linux/kprobes.h>
#include <linux/bpf.h>
#include <linux/filter.h>

#include "skel_internal.h"



MODULE_LICENSE("GPL");
MODULE_AUTHOR("yeop");
MODULE_DESCRIPTION("A Simple Hello World module");

#define PROG_NAME_SIZE 64

int ebpf_load(u8 *prog_name, u8 *buf, u32 prog_size, u8 *log_buf, u32 log_buf_size);

static int __init hello_init(void)
{

    printk(KERN_INFO "Hello world!\n");
    int efd = -1;

    char kernel_buf[1024] = {
        0xb7, 0x01, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00,
        0x6b, 0x1a, 0xfc, 0xff, 0x00, 0x00, 0x00, 0x00,
        0xb7, 0x01, 0x00, 0x00, 0x72, 0x6c, 0x64, 0x21,
        0x63, 0x1a, 0xf8, 0xff, 0x00, 0x00, 0x00, 0x00,
        0x18, 0x01, 0x00, 0x00, 0x48, 0x65, 0x6c, 0x6c,
        0x00, 0x00, 0x00, 0x00, 0x6f, 0x20, 0x57, 0x6f,
        0x7b, 0x1a, 0xf0, 0xff, 0x00, 0x00, 0x00, 0x00,
        0xbf, 0xa1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x07, 0x01, 0x00, 0x00, 0xf0, 0xff, 0xff, 0xff,
        0xb7, 0x02, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x00,
        0x85, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00,
        0xb7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x95, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    };
    int program_size = 104;

    struct bpf_load_and_run_opts opts = {};
	struct bpf_loader_ctx *ctx;
    struct bpf_insn *insn;
	int ctx_sz = sizeof(*ctx);
	int err;
    char errstr[128];
    char data[128];
	char log_buf[128];

    //char *data;
    //data = kmalloc(128, GFP_KERNEL);

    memset(errstr, 0, 128);
    memset(data, 0, 128);
    memset(log_buf, 0, 128);

	ctx = kmalloc(ctx_sz, GFP_KERNEL);
    memset(ctx, 0, ctx_sz);
    ctx->sz = ctx_sz;
    
    insn = (struct bpf_insn*)kernel_buf;
	opts.ctx = ctx;
    opts.data = (void*)data;
    opts.data_sz = 128;
    opts.insns =(void*)insn;
    opts.insns_sz = program_size;
    //opts.errstr = errstr;

    efd = bpf_load_and_run(&opts);

    //printk(KERN_INFO "error: %s\n", errstr);
    printk(KERN_INFO "efd: %d\n", efd);

    kfree(ctx);

    return 0;    // Non-zero return means that the module couldn't be loaded.
}


static void __exit hello_cleanup(void)
{
    printk(KERN_INFO "Cleaning up module.\n");
}

module_init(hello_init);
module_exit(hello_cleanup);
