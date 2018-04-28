#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/pid.h>
#include <linux/mm.h>

static int processid;
static int is_stack(struct vm_area_struct *vma);
static int is_heap(struct vm_area_struct *vma);
void print_memory_area(struct mm_struct *mm, struct vm_area_struct *vma);
void print_page_tables(struct mm_struct *mm);

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
  print_page_tables(mm);

  return 0;
}

void cleanup_module(void)
{
  printk(KERN_INFO "Module removed\n");
}

void print_memory_area(struct mm_struct *mm, struct vm_area_struct *vma) {

  printk("Code start: %lx\t end: %lx\t size: %lu\n", mm->start_code,
    mm->end_code, mm->end_code - mm->start_code);
  printk("Data start: %lx\t end: %lx\t size: %lu\n",
    mm->start_data, mm->end_data, mm->end_data - mm->start_data);

  for (; vma != NULL; vma = vma->vm_next) {
    if (is_stack(vma)) {
      printk("Stack: start: %lx\t end: %lx\t size: %lu\n",
       vma->vm_start, vma->vm_end, vma->vm_end - vma->vm_start);
    }else if(is_heap(vma)){
      printk("Heap: start: %lx\t end: %lx\t size: %lu\n",
       vma->vm_start, vma->vm_end, vma->vm_end - vma->vm_start);
    }
  }

  printk("Main Arguments start: %lx\t end: %lx\t size: %lu\n",
    mm->arg_start, mm->arg_end, mm->arg_end - mm->arg_start);
  printk("Environment Variables start: %lx\t end: %lx\t size: %lu\n",
    mm->env_start, mm->env_end, mm->env_end - mm->env_start);
  printk("number of frames used by the process (rss): %lx\t", get_mm_rss(mm));
  printk("Total virtual memory used by the process (total_vm): %lx\t", mm->total_vm);
}

void print_page_tables(struct mm_struct *mm) {
  int i;
  pgd_t *pgd = mm->pgd;
  for (i = 0; i < 512; i++) {
    unsigned long address;
    address = pgd[i].pgd;
    if ( address != 0) {
      printk("\nEntry %d", i);
      printk("PGD: %#018lx\t", address);
      printk("P: %lu\t", (address << 63) >> 63);
      printk("R/W: %lu\t", (address << 62) >> 63);
      printk("U/S: %lu\t", (address << 61) >> 63);
      printk("PWT: %lu\t", (address << 60) >> 63);
      printk("PCD: %lu\t", (address << 59) >> 63);
      printk("A: %lu\t", (address << 58) >> 63);
      printk("PS: %lu\t", (address << 56) >> 63);
      printk("Physical adress: %#08lx\t", (address << 28) >> 40);
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
