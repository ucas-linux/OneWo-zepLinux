Hardware timer ISR latency
##########################

This sample measures hardware timer interrupt response more directly than the
``cyclictest``-style sample.

Method
******

- Use STM32 ``TIM2`` counter device as an alarm source.
- Record the expected alarm expiry in counter ticks.
- In the alarm callback, immediately read ``k_cycle_get_32()``.
- Compute latency from expected expiry to callback execution.
- Store ISR results in globals; printing and statistics run in thread context.

Compared with the thread wakeup sample, this removes most scheduler wakeup
latency from the measurement path and is closer to timer interrupt response.

Notes
*****

The callback is still mediated by the Zephyr counter driver, so this is not a
raw assembly ISR-entry benchmark. But it is significantly closer to actual
interrupt response than measuring a resumed thread.
