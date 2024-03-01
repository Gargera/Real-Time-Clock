#ifndef TIMER_H_
#define TIMER_H_

void timer0_CTC_init_interrupt(void);
void timer0_wave_nonPWM(void);
void timer0_wave_fastPWM(void);
void timer0_wave_phasecorrectPWM(void);
void timer2_overflow_init_interrupt(void);

#endif /* TIMER_H_ */