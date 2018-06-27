#include <linux/linkage.h>
#include <linux/kernel.h>
#include <linux/module.h>
asmlinkage void sched_faster(int f_pid);
asmlinkage void sched_slower(int s_pid);

pid_t fast = 0;
pid_t slow = 0;
asmlinkage void sched_faster(int f_pid)
{
	fast = f_pid;
}

asmlinkage void sched_slower(int s_pid)
{
	slow = s_pid;
}

EXPORT_SYMBOL(fast);
EXPORT_SYMBOL(slow);
