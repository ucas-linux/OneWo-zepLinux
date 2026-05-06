IRQ latency cyclictest style
############################

This sample measures timer wakeup latency similarly to ``cyclictest``.
It arms a one-shot ``k_timer`` periodically, records the expected wakeup
cycle count before starting the timer, then measures the delay when the
kernel timer expiry handler releases a semaphore and the measurement thread
runs.

Metrics
*******

- current latency in cycles and ns
- minimum latency
- maximum latency
- average latency

Notes
*****

This is not a pure hardware ISR-entry latency measurement. It includes:

- system timer interrupt handling
- kernel timer expiry processing
- scheduler wakeup latency for the measuring thread

So it is closer to ``cyclictest`` wakeup latency than bare interrupt entry
latency.
