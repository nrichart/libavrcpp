#ifndef __TIMER_INTERRUPT_HH__
#define __TIMER_INTERRUPT_HH__

ISR(TIMER0_OVF_vect) {
  Timer0::userOverflowInterupt();
}

ISR(TIMER1_OVF_vect) {
  Timer1::userOverflowInterupt();
}

ISR(TIMER2_OVF_vect) {
  Timer2::userOverflowInterupt();
}


#endif /* __TIMER_INTERRUPT_HH__ */
