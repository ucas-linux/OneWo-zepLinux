# LMZ Design Principles

## Overview

This document outlines the core design principles that guide LMZ implementation and evolution.

## Core Principles

### 1. Correctness First

LMZ prioritizes correctness over performance. All implementations must:
- Satisfy pre/post-conditions from SYSSPEC specifications
- Maintain invariants at all times
- Handle all error cases explicitly
- Avoid undefined behavior

### 2. Platform Awareness

LMZ is designed specifically for Cortex-M3 and Zephyr RTOS:
- Leverage hardware features (MPU, NVIC, SysTick)
- Respect Zephyr's design patterns (static allocation, callbacks)
- Optimize for memory-constrained environments
- Consider interrupt context constraints

### 3. Formally Specified

All interfaces are specified using SYSSPEC methodology:
- Hoare-logic pre/post-conditions
- Explicit invariants
- System algorithms
- Rely/Guarantee clauses
- Concurrency specifications

### 4. AI-Assisted with Human Fine-Tuning

Development workflow:
1. AI generates code from SYSSPEC specifications
2. Human reviews and optimizes for:
   - Platform-specific optimizations
   - Resource constraints
   - Edge cases
   - Code style consistency

### 5. Composability

Modules must be composable:
- Clear Rely clauses (what we assume)
- Clear Guarantee clauses (what we promise)
- Minimal dependencies
- Well-defined boundaries

### 6. Testability

Every interface must be testable:
- Unit tests for all pre/post-conditions
- Integration tests for module interaction
- Stress tests for resource limits
- Property-based tests for invariants

## Architectural Decisions

### Why MPU Instead of MMU?

Cortex-M3 lacks MMU, so LMZ uses MPU for process isolation:
- **Trade-off**: No copy-on-write, full stack copies required
- **Benefit**: Hardware-enforced isolation on resource-constrained MCU
- **Implementation**: Static MPU regions per process

### Why Threads Instead of Processes?

Zephyr uses threads, not processes:
- **Adaptation**: Simulate processes using threads + MPU
- **Resource tracking**: Global table for cleanup
- **Fork behavior**: Full context copy (not COW)

### Why Priority Inversion?

Priority inheritance is critical for real-time systems:
- **Zephyr support**: k_mutex provides priority inheritance
- **Implementation**: Use Zephyr k_mutex directly
- **Result**: No priority inversion issues

## Future Considerations

### Extensibility

- **New interfaces**: Add via same SYSSPEC pattern
- **New platforms**: Abstract hardware dependencies
- **New schedulers**: Allow alternative scheduling algorithms

### Performance

- **Hot paths**: Optimize critical sections
- **Cache efficiency**: Consider data layout
- **Lock granularity**: Minimize lock contention

### Safety

- **MPU regions**: Ensure proper isolation
- **Stack guards**: Detect overflow
- **System calls**: Validate all user parameters

## References

- [AI Interface Specifications](../ai/interface-specifications.md)
- [SYSSPEC Methodology](../human/sysspec-reference.md)
- [Architecture Overview](../human/architecture.md)
