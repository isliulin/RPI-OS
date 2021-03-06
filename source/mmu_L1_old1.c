#include "../include/mmu.h"

extern void PUT32(unsigned int , unsigned int);
extern unsigned int GET32(unsigned int);
extern unsigned int __end;

void mmu_section(unsigned int mmu_base, unsigned int vadd, unsigned int padd, unsigned int flags)
{
	unsigned int ra;
	unsigned int rb;
	unsigned int rc;

	ra = vadd>>20;
	rb = ((mmu_base&PAGE_TABLE_L1_BASE_ADDR_MASK) | (ra<<2));
	ra = padd>>20;
	rc = ((ra<<20) | flags | 2);
	PUT32(rb, rc);
}

void init_sys_mmu(void)
{
	int i;
	unsigned int mmu_base = (unsigned int)(&__end);
	for(i = 0; i < (MEM_MAP_SIZE>>20); i++)
	{
		mmu_section(mmu_base, VIRTUAL_MEM_ADDR+(i<<20), PHYSICAL_MEM_ADDR+(i<<20), 0x00000);
	}

	for(i = 0; i < (IO_MAP_SIZE>>20); i++)
	{
		mmu_section(mmu_base, VIRTUAL_IO_ADDR+(i<<20), PHYSICAL_IO_ADDR+(i<<20), 0x00000);
	}
}

void start_mmu(void)
{
	unsigned int ttb = (unsigned int)(&__end);
	asm(
		"mcr p15,0,%0,c2,c0,0\n"
		"mvn r0,#0\n"
		"mcr p15,0,r0,c3,c0,0\n"
		"mov r0,#0x1\n"
		"mcr p15,0,r0,c1,c0,0\n"
		"mov r0,r0\n"
		"mov r0,r0\n"
		"mov r0,r0\n"
		:
		: "r" (ttb)
		: "r0"
	);
}
