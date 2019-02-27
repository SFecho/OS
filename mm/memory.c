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
        p++;
        if (p->type > 4)
            break;
    }
    printk("OS available RAM is %ldB\n", total_mem);
}