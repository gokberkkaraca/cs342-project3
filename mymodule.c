#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/pid.h>

static int processid;
static int is_stack(struct vm_area_struct *vma);
static int is_heap(struct vm_area_struct *vma);
void print_memory_area(struct mm_struct *mm, struct vm_area_struct *vma);

int init_module(void)
{
  struct task_struct *task;
  struct pid *pid_struct;
  struct mm_struct *mm;
  struct vm_area_struct *vma;

  printk(KERN_INFO "Module added\n");
  printk("Looking for process ID: %d\n", processid);

  pid_struct = find_get_pid(processid);
  task = NULL;
  task = pid_task(pid_struct, PIDTYPE_PID);

  mm = task->mm;
  vma = mm->mmap;
  print_memory_area(mm, vma);

	return 0;
}

void cleanup_module(void)
{
	printk(KERN_INFO "Module removed\n");
}

void print_memory_area(struct mm_struct *mm, struct vm_area_struct *vma) {

  printk("Code start: %lx\t end: %lx\t size: %lx\n", mm->start_code,
    mm->end_code, mm->end_code - mm->start_code);
  printk("Data start: %lx\t end: %lx\t size: %lx\n",
    mm->start_data, mm->end_data, mm->end_data - mm->start_data);

  for (; vma != NULL; vma = vma->vm_next) {
    if (is_stack(vma)) {
      printk("Stack: start: %lx\t end: %lx\t size: %lx\n",
       vma->vm_start, vma->vm_end, vma->vm_end - vma->vm_start);
    }else if(is_heap(vma)){
      printk("Heap: start: %lx\t end: %lx\t size: %lx\n",
       vma->vm_start, vma->vm_end, vma->vm_end - vma->vm_start);
    }
  }

}

/* This method is taken from linux kernel source code */
static int is_stack(struct vm_area_struct *vma)
{
	return vma->vm_start <= vma->vm_mm->start_stack &&
		vma->vm_end >= vma->vm_mm->start_stack;
}

static int is_heap(struct vm_area_struct *vma)
{
  return vma->vm_start <= vma->vm_mm->brk &&
    vma->vm_end >= vma->vm_mm->start_brk;
}

module_param(processid, int, 0);
MODULE_LICENSE("Dual BSD/GPL");
