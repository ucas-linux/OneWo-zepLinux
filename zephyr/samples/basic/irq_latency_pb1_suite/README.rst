Rocket Pi PB1 PB2 latency suite
###############################

This sample combines:

- one-shot ``PB1 -> PB2`` low check
- one-shot ``PB1 -> PB2`` high check
- 20 hardware timer ISR-latency samples on ``TIM2``
- 20 ``k_timer`` wakeup-latency samples
