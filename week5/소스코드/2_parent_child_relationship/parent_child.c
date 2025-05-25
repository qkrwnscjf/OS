#include <linux/module.h>
#include <linux/printk.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>

static int __init module_start(void) {
	struct task_struct *task;
	struct task_struct *children;
	struct list_head *list;

	pr_info("INSTALL : parent child \n");

	// 1) init_task process로 시작해서
	//    모든 프로세스를 탐색한다
	for_each_process(task) {
		// 2) 해당 프로세스의 task 구조체 정보를 출력한다
		pr_info("\n %4d task %s\n children: ",
				task->pid, task->comm);
		// 3) 해당 task의 자식 head를 가져온다
		list_for_each(list, &task->children) {
			// 4) 자식 head를 기준으로 형제 관계(sibling)에
			//    있는 모든 자식들을 불어온다
			children = list_entry(list, struct task_struct, sibling);
			// 5) 자식의 정보를 출력한다
			pr_info("- %4d %s", children->pid, children->comm);
		}
	}

	return 0;
}

static void __exit module_end(void) {
	pr_info("REMOVE: parent child .. \n");
}

module_init(module_start);
module_exit(module_end);
MODULE_LICENSE("GPL");
