#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/module.h>
#include <linux/sched.h>

asmlinkage long sys_hello(void) {
 printk("Hello, World!\n");
 return 0;
}

asmlinkage long sys_set_weight(int weight){
  if(weight < 0){
    return -EINVAL;
  }
  current->weight = weight;
  
  return 0;
}

asmlinkage long sys_get_weight(void){

  int weight = current->weight ;
  return weight;
}


int traverse_siblings_sum_weight(struct task_struct *root_task) {
	
	struct task_struct *parent_task = root_task->parent;
	struct task_struct *task;
	struct list_head *list;
	int sum = 0;
	int siblings_count = 0;

	if (root_task->pid == 1)
	    return -ESRCH;

	// Traverse through each sibling
	list_for_each(list, &parent_task->children) {
	    task = list_entry(list, struct task_struct, sibling);
	    if (task != root_task) {
	        // Add the weight of the sibling to the sum
	        sum += task->weight;
	        siblings_count++;
	    }
	}

	if (siblings_count == 0)
    		return -ESRCH;

	return sum;
}


asmlinkage long sys_get_siblings_sum(void){
	
  return traverse_siblings_sum_weight(current);
  
}


pid_t find_smallest_divisible_ancestor_pid(void) {
    struct task_struct *ancestor_task = current;
    int min_weight = ancestor_task->weight; // Initialize min_weight with weight of current process
    pid_t min_pid = ancestor_task->pid; // Initialize min_pid with PID of current process

	if (ancestor_task->weight == 0){
		return min_pid;
	}

    // Traverse through ancestors
    while (ancestor_task->pid > 1) {
        ancestor_task = ancestor_task->parent;

        // Check if weight divides the weight of the calling process
        if (ancestor_task->weight != 0 && current->weight % ancestor_task->weight == 0) {
            // Update min_weight and min_pid if current ancestor's weight is smaller
            if (ancestor_task->weight < min_weight) {
                min_weight = ancestor_task->weight;
                min_pid = ancestor_task->pid;
            }
       	 }
    }
    return min_pid;
}


asmlinkage pid_t sys_get_lightest_divisor_ancestor(void){
	
  return find_smallest_divisible_ancestor_pid();
  
}



