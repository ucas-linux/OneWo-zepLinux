#ifndef AS32X601_THREAD_PROBE_H_
#define AS32X601_THREAD_PROBE_H_

#include <stdint.h>

extern volatile uint32_t as32x601_thread_probe_stage;

void as32x601_thread_probe_mark(uint32_t stage);

#endif
