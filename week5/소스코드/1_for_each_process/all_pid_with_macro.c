#include <linux/module.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>
#include <linux/printk.h>

static int __init module_start(void) {
	
	struct task_struct *task;

	pr_info("Init Module ... \n");

	// init_task process 부터 실행된 모든 
	// 프로세스 정보 출력
	for_each_process(task) {
		pr_info("%s[%d] : %d\n",
			task->comm, task->pid, task->__state);
	}

	return 0;
}

static void __exit module_end(void) {
	pr_info("Module removing ... \n"); 
}

module_init(module_start);
module_exit(module_end);
MODULE_LICENSE("GPL");


