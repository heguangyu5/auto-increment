// @see /fs/proc/uptime.c?v=linux-2.6.39

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

static atomic64_t value;
static long init_value = 0;
module_param(init_value, long, 0);

struct proc_dir_entry *ai_file;

static int inc(struct seq_file *m, void *v)
{
    seq_printf(m, "%lu\n", atomic64_inc_return(&value));
    return 0;
}

static int ai_open(struct inode *inode, struct file *file)
{
    return single_open(file, inc, NULL);
}

static const struct file_operations ai_proc_fops = {
    .open    = ai_open,
    .read    = seq_read,
    .llseek  = seq_lseek,
    .release = single_release
};

static int ai_init(void)
{
    atomic64_set(&value, init_value);
    printk(KERN_ALERT "/proc/auto-increment init value %ld\n", atomic64_read(&value));

    ai_file = proc_create("auto-increment", 0, NULL, &ai_proc_fops);
    printk(KERN_ALERT "/proc/auto-increment created\n");
    return 0;
}

static void ai_exit(void)
{
    proc_remove(ai_file);
    printk(KERN_ALERT "/proc/auto-increment removed\n");
}

module_init(ai_init);
module_exit(ai_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("heguangyu5@qq.com");
MODULE_DESCRIPTION("A MySQL auto-increment like module");
