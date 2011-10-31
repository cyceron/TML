#include "armint.h"

#define IRQ_MASK 0x00000080
#define FIQ_MASK 0x00000040
#define INT_MASK (IRQ_MASK|FIQ_MASK)


static inline cpu_t get_cpsr(void)
{
  cpu_t val;
  asm volatile ("mrs %[val], cpsr\n":[val]"=r"(val):);
  return val;
}

static inline void set_cpsr(cpu_t val)
{
	asm volatile ("msr  cpsr, %[val]\n" ::[val]"r"(val)  );
}

cpu_t disable_irq(void)
{
	cpu_t cpsr;
	cpsr = get_cpsr();
	set_cpsr(cpsr | IRQ_MASK);
	return cpsr;
}

cpu_t enable_irq(void)
{
	cpu_t cpsr;
	cpsr = get_cpsr();
	set_cpsr(cpsr & ~IRQ_MASK);
	return cpsr;
}

cpu_t restore_irq(cpu_t old_cpsr)
{
	cpu_t cpsr;
	cpsr = get_cpsr();
	set_cpsr( (cpsr & ~IRQ_MASK) | (old_cpsr & IRQ_MASK) );
	return cpsr;
}

cpu_t disable_fiq(void)
{
	cpu_t cpsr;
	cpsr = get_cpsr();
	set_cpsr(cpsr | FIQ_MASK);
	return cpsr;
}

cpu_t enable_fiq(void)
{
	cpu_t cpsr;
	cpsr = get_cpsr();
	set_cpsr(cpsr & ~FIQ_MASK);
	return cpsr;
}

cpu_t restore_fiq(cpu_t old_cpsr)
{
	cpu_t cpsr;
	cpsr = get_cpsr();
	set_cpsr( (cpsr & ~FIQ_MASK) | (old_cpsr & FIQ_MASK) );
	return cpsr;
}

