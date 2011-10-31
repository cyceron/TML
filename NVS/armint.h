#ifndef ARMINT_H_
#define ARMINT_H_

typedef unsigned long cpu_t;
#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/
cpu_t disable_irq(void);
cpu_t enable_irq(void);
cpu_t restore_irq(cpu_t old_cpsr);
cpu_t disable_fiq(void);
cpu_t enable_fiq(void);
cpu_t restore_fiq(cpu_t old_cpsr);

#ifdef __cplusplus
}
#endif /*__cplusplus*/


#endif /*ARMINT_H_*/
