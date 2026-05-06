Direct TIM2 IRQ handler latency
###############################

This sample measures timer interrupt response by timestamping directly inside a
custom ``TIM2`` IRQ handler.

Notes
*****

- This is closer to raw IRQ response than thread wakeup latency.
- It still includes NVIC entry and ISR prologue cost.
- It bypasses the Zephyr counter driver for the measurement path.
