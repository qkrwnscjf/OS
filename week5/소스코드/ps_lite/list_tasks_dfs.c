#include <linux/module.h>
#include <linux/printk.h>
#include <linux/sched.h>

/* Depth-First Search Algorithm */
static void dfs(struct task_struct *task) {
	struct task_struct *child;
	struct list_head *list;
	
	pr_info("COMM: %-20s STATE: %d\tPID: %d\n",
		task->comm, task->__state, task->pid);

	// TODO 3: dfs 구현하기
	list_for_each(list, &task -> children) { // 자식 프로세스 리스트 순회 후 dfs호출
		child = list_entry(list, struct task_struct, sibling); // list_entry로 자식 프로세스 접근
		dfs(child); // 재귀 호출 , depth 설정 여부?
	}
}

static int __init list_task_init(void) {
	struct task_struct *init_task;
	struct pid *init_pid;

	// TODO 1: init 프로세스의 pid 구조체 가져오기
	// *      (find_get_pid 사용)
	init_pid = find_get_pid(1); 

	// TODO 2: pid 구조체를 통해, 해당 프로세스의 task_struct 
	//         가져오기 
	// *     (pid_task 사용) 
	init_task = pid_task(init_pid, PIDTYPE_PID); 

	pr_info("INSTALL: list_tasks_dfs module ..\n");

	
	dfs(init_task);

	return 0;
}

static void __exit list_task_exit(void) {
	pr_info("REMOVE: list_tasks_dfs module .. \n");
}

module_init(list_task_init);
module_exit(list_task_exit);
MODULE_LICENSE("GPL");


