#include <os/kernel.h>
#include <os/memory.h>
#include <stddef.h>

void init_memory()
{
    int i;
    memory_e820 *p = (memory_e820 *)0xffff800000007e00;
    uint64 total_mem = 0;
    printk("physics memory map:\n1. RAM, 2:ROM or reserved, 3:ACPI reclaim memory, 4:ACPI NVS memory, other:undefined\n");
    for (i = 0; i < 32; i++)
    {
        printk("address:%#018lx\tlength:%#018lx\ttype:%#018lx\n", p->addr, p->length, p->type);
        if (p->type == 1)
            total_mem += p->length;
        memory_manage_struct.e820[i].addr = p->addr;
        memory_manage_struct.e820[i].length = p->length;
        memory_manage_struct.e820[i].type = p->type;
        memory_manage_struct.e820_length = i + 1;
        p++;
        if (p->type > 4)
            break;
    }
 
    printk("OS available RAM is %ldB\n", total_mem);
    
    total_mem = 0;

    for(i = 0; i < memory_manage_struct.e820_length; i++){
        uint64 start, end;       
        if(memory_manage_struct.e820[i].type != 1)
            continue;
        start = PAGE_2M_ALIGN(memory_manage_struct.e820[i].addr);
        end = ((memory_manage_struct.e820[i].addr + memory_manage_struct.e820[i].length) >> PAGE_2M_NR)
                << PAGE_2M_NR;
        
        printk("%#x, %#x\n", start, end);
       
        if(end <= start)
            continue;
        
        total_mem += (end-start) >> PAGE_2M_NR;
   } 

    printk("OS can use 2MB pages is %#010x=%010d\n", total_mem, total_mem);
}