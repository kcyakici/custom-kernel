#include <linux/init.h> // for the macros
#include <linux/module.h> // used by all modules
#include <linux/kernel.h> // for KERN_INFO
#include <linux/sched.h>
#include <linux/sched/mm.h>
#include <linux/string.h>
#include <linux/syscalls.h>
#include <linux/unistd.h>
#include <linux/proc_fs.h>

SYSCALL_DEFINE1(get_memory_usage, int, pid) {

	struct task_struct *task; // a struct defined for the processes
	struct mm_struct *mm; // memory descriptor of the task structure
	unsigned long rss;

	for_each_process(task) { // it works like a for loop for the working processes
		if (task_pid_nr(task) == pid) { // task_pid_nr returns the pid of the process
			mm = get_task_mm(task); // get the address space of the task, returns NULL if no mm
			if (mm) {
				rss = get_mm_rss(mm) << PAGE_SHIFT; // get_mm_rss returns the number of pages, so we have to multiply this by the page size (equivalent to shifting with PAGE_SHIFT)
				mmput(mm); // to release the mm we have gotten with get_task_mm(), this operation is used
				return rss/1024; // the result returns bytes, so I am dividing it to get the same answer with "top" command
        		}
    		}
	}
	return -1; // implementing this for a possible error check in future implementations
}
