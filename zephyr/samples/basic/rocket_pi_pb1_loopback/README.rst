Rocket Pi PB1 loopback
#####################

This sample drives ``PB1`` as a GPIO output and reads ``PB2`` as a GPIO input
for a simple interconnect sanity test.

Wiring
******

This sample assumes ``PB1`` is electrically connected to ``PB2``.

Behavior
********

The application drives ``PB1`` once, samples ``PB2`` once, and prints whether
the sampled input matches the driven level.
