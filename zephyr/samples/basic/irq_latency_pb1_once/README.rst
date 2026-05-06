Rocket Pi PB1 one-shot loopback with IRQ latency
################################################

This sample combines the ``rocket_pi_pb1_loopback`` GPIO sanity check with the
``irq_latency_isr`` timer measurement.

Behavior
********

- Drive ``PB1`` high once.
- Sample the connected ``PB2`` input once and print ``PASS`` or ``FAIL``.
- Arm a single ``TIM2`` counter alarm.
- Measure one hardware timer ISR callback latency sample.
- Print the latency result once and exit.

Wiring
******

This sample assumes the Rocket Pi board has ``PB1`` connected to ``PB2``.
