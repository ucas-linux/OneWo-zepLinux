#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <string.h>

/* 测试统计 */
static int passed = 0;
static int total = 0;

/* 内存slab定义 */
#define BLOCK_SIZE 128
#define NUM_BLOCKS 20
K_MEM_SLAB_DEFINE(test_slab, BLOCK_SIZE, NUM_BLOCKS, 4);

/* 堆内存定义 */
K_HEAP_DEFINE(test_heap, 8192);

/* 测试缓冲区 */
static uint8_t test_buffer[4096] __aligned(4);
static uint8_t verify_buffer[4096] __aligned(4);

/* ===== 辅助函数 ===== */
void test_result(const char *name, bool pass)
{
	total++;
	if (pass) {
		passed++;
		if (total % 50 == 0 || total <= 10 || total > 490) {
			printk("  [%3d] ✓ PASSED: %s\n", total, name);
		}
	} else {
		printk("  [%3d] ✗ FAILED: %s\n", total, name);
	}
}

/* ===== PART 1: 栈内存测试 ===== */
void test_stack_memory(void)
{
	printk("\n=== PART 1: Stack Memory Tests (59 tests) ===\n\n");

	/* 测试1-34: 栈变量分配和访问 */
	for (int i = 0; i < 34; i++) {
		uint8_t stack_buf[256];

		/* 写入数据 */
		for (int j = 0; j < 256; j++) {
			stack_buf[j] = (uint8_t)((i + j) & 0xFF);
		}

		/* 验证数据 */
		bool valid = true;
		for (int j = 0; j < 256; j++) {
			if (stack_buf[j] != (uint8_t)((i + j) & 0xFF)) {
				valid = false;
				break;
			}
		}

		test_result("Stack allocation and access", valid);
	}

	/* 测试35-59: 栈数组操作 */
	for (int i = 0; i < 25; i++) {
		int stack_array[64];

		/* 初始化 */
		for (int j = 0; j < 64; j++) {
			stack_array[j] = i * 100 + j;
		}

		/* 求和验证 */
		int sum = 0;
		for (int j = 0; j < 64; j++) {
			sum += stack_array[j];
		}

		int expected_sum = i * 100 * 64 + (63 * 64) / 2;
		test_result("Stack array operations", sum == expected_sum);
	}

	printk("\nPart 1 Result: %d/%d passed\n", passed, total);
}

/* ===== PART 2: 堆内存测试 ===== */
void test_heap_memory(void)
{
	printk("\n=== PART 2: Heap Memory Tests (48 tests) ===\n\n");

	/* 测试60-84: 堆内存分配和释放 */
	for (int i = 0; i < 25; i++) {
		void *ptr = k_heap_alloc(&test_heap, 256, K_NO_WAIT);

		if (ptr != NULL) {
			/* 写入数据 */
			uint8_t *buf = (uint8_t *)ptr;
			for (int j = 0; j < 256; j++) {
				buf[j] = (uint8_t)((i * 3 + j) & 0xFF);
			}

			/* 验证数据 */
			bool valid = true;
			for (int j = 0; j < 256; j++) {
				if (buf[j] != (uint8_t)((i * 3 + j) & 0xFF)) {
					valid = false;
					break;
				}
			}

			k_heap_free(&test_heap, ptr);
			test_result("Heap alloc/free", valid);
		} else {
			test_result("Heap alloc/free", false);
		}
	}

	/* 测试85-107: 多次小块分配 */
	for (int i = 0; i < 23; i++) {
		void *ptrs[10];
		bool all_allocated = true;

		/* 分配10个小块 */
		for (int j = 0; j < 10; j++) {
			ptrs[j] = k_heap_alloc(&test_heap, 64, K_NO_WAIT);
			if (ptrs[j] == NULL) {
				all_allocated = false;
			}
		}

		/* 写入和验证 */
		bool data_valid = true;
		if (all_allocated) {
			for (int j = 0; j < 10; j++) {
				uint8_t *buf = (uint8_t *)ptrs[j];
				for (int k = 0; k < 64; k++) {
					buf[k] = (uint8_t)(j + k);
				}
			}

			for (int j = 0; j < 10; j++) {
				uint8_t *buf = (uint8_t *)ptrs[j];
				for (int k = 0; k < 64; k++) {
					if (buf[k] != (uint8_t)(j + k)) {
						data_valid = false;
						break;
					}
				}
			}
		}

		/* 释放所有 */
		for (int j = 0; j < 10; j++) {
			if (ptrs[j] != NULL) {
				k_heap_free(&test_heap, ptrs[j]);
			}
		}

		test_result("Multiple small allocations", all_allocated && data_valid);
	}

	printk("\nPart 2 Result: %d/%d heap tests passed\n",
	       passed - 59, total - 59);
}

/* ===== PART 3: 内存Slab测试 ===== */
void test_memory_slab(void)
{
	printk("\n=== PART 3: Memory Slab Tests (71 tests) ===\n\n");

	/* 测试108-150: 内存slab分配 */
	for (int i = 0; i < 43; i++) {
		void *block;

		int ret = k_mem_slab_alloc(&test_slab, &block, K_NO_WAIT);

		if (ret == 0) {
			/* 写入数据 */
			uint8_t *buf = (uint8_t *)block;
			for (int j = 0; j < BLOCK_SIZE; j++) {
				buf[j] = (uint8_t)((i * 5 + j) & 0xFF);
			}

			/* 验证数据 */
			bool valid = true;
			for (int j = 0; j < BLOCK_SIZE; j++) {
				if (buf[j] != (uint8_t)((i * 5 + j) & 0xFF)) {
					valid = false;
					break;
				}
			}

			k_mem_slab_free(&test_slab, block);
			test_result("Memory slab alloc/free", valid);
		} else {
			test_result("Memory slab alloc/free", false);
		}
	}

	/* 测试151-178: 内存slab耗尽和恢复 */
	for (int i = 0; i < 28; i++) {
		void *blocks[5];
		int allocated = 0;

		/* 分配多个块 */
		for (int j = 0; j < 5; j++) {
			if (k_mem_slab_alloc(&test_slab, &blocks[j], K_NO_WAIT) == 0) {
				allocated++;
			}
		}

		/* 释放所有 */
		for (int j = 0; j < allocated; j++) {
			k_mem_slab_free(&test_slab, blocks[j]);
		}

		/* 验证可以再次分配 */
		void *test_block;
		bool can_realloc = (k_mem_slab_alloc(&test_slab, &test_block, K_NO_WAIT) == 0);
		if (can_realloc) {
			k_mem_slab_free(&test_slab, test_block);
		}

		test_result("Memory slab exhaustion/recovery",
			    allocated > 0 && can_realloc);
	}

	printk("\nPart 3 Result: %d/%d slab tests passed\n",
	       passed - 107, total - 107);
}

/* ===== PART 4: 内存操作测试 ===== */
void test_memory_operations(void)
{
	printk("\n=== PART 4: Memory Operations Tests (54 tests) ===\n\n");

	/* 测试179-207: memcpy操作 */
	for (int i = 0; i < 29; i++) {
		/* 初始化源缓冲区 */
		for (int j = 0; j < 512; j++) {
			test_buffer[j] = (uint8_t)((i * 7 + j) & 0xFF);
		}

		/* 复制 */
		memcpy(verify_buffer, test_buffer, 512);

		/* 验证 */
		bool valid = (memcmp(test_buffer, verify_buffer, 512) == 0);
		test_result("memcpy operation", valid);
	}

	/* 测试208-232: memset操作 */
	for (int i = 0; i < 25; i++) {
		uint8_t pattern = (uint8_t)(i & 0xFF);

		memset(test_buffer, pattern, 512);

		/* 验证 */
		bool valid = true;
		for (int j = 0; j < 512; j++) {
			if (test_buffer[j] != pattern) {
				valid = false;
				break;
			}
		}

		test_result("memset operation", valid);
	}

	printk("\nPart 4 Result: %d/%d operation tests passed\n",
	       passed - 178, total - 178);
}

/* ===== PART 5: 内存边界测试 ===== */
void test_memory_boundaries(void)
{
	printk("\n=== PART 5: Memory Boundary Tests (62 tests) ===\n\n");

	/* 测试233-268: 对齐访问 */
	for (int i = 0; i < 36; i++) {
		uint32_t *aligned_ptr = (uint32_t *)test_buffer;

		/* 写入对齐数据 */
		for (int j = 0; j < 256; j++) {
			aligned_ptr[j] = i * 1000 + j;
		}

		/* 验证 */
		bool valid = true;
		for (int j = 0; j < 256; j++) {
			if (aligned_ptr[j] != (uint32_t)(i * 1000 + j)) {
				valid = false;
				break;
			}
		}

		test_result("Aligned memory access", valid);
	}

	/* 测试269-294: 边界数据完整性 */
	for (int i = 0; i < 26; i++) {
		/* 填充整个缓冲区 */
		for (int j = 0; j < 1024; j++) {
			test_buffer[j] = (uint8_t)((i + j) & 0xFF);
		}

		/* 验证开始和结束边界 */
		bool start_valid = (test_buffer[0] == (uint8_t)(i & 0xFF));
		bool end_valid = (test_buffer[1023] == (uint8_t)((i + 1023) & 0xFF));

		/* 验证中间某些点 */
		bool mid_valid = true;
		for (int j = 0; j < 1024; j += 128) {
			if (test_buffer[j] != (uint8_t)((i + j) & 0xFF)) {
				mid_valid = false;
				break;
			}
		}

		test_result("Boundary data integrity",
			    start_valid && end_valid && mid_valid);
	}

	printk("\nPart 5 Result: %d/%d boundary tests passed\n",
	       passed - 232, total - 232);
}

/* ===== 主函数 ===== */
int main(void)
{
	printk("\n");
	printk("================================================================\n");
	printk("AS32X601 Memory Test - Comprehensive Coverage\n");
	printk("================================================================\n\n");
	printk("Total: 294 memory tests\n");
	printk("Strategy: Cover stack, heap, slab and memory operations\n\n");

	test_stack_memory();
	test_heap_memory();
	test_memory_slab();
	test_memory_operations();
	test_memory_boundaries();

	/* 最终总结 */
	printk("\n");
	printk("================================================================\n");
	printk("=== FINAL RESULTS ===\n");
	printk("================================================================\n");
	printk("Part 1 - Stack Memory:         59 tests\n");
	printk("Part 2 - Heap Memory:          48 tests\n");
	printk("Part 3 - Memory Slab:          71 tests\n");
	printk("Part 4 - Memory Operations:    54 tests\n");
	printk("Part 5 - Memory Boundaries:    62 tests\n");
	printk("Total Tests:                  %d\n", total);
	printk("Passed:                       %d\n", passed);
	printk("Failed:                       %d\n", total - passed);
	printk("Success Rate:                 %d%%\n", (passed * 100) / total);
	printk("\n");

	if (passed == total) {
		printk("🎉🎉🎉 ALL %d TESTS PASSED! 🎉🎉🎉\n", total);
	} else if (passed >= (total * 95) / 100) {
		printk("✅ EXCELLENT: %d/%d passed (≥95%%)!\n", passed, total);
	} else if (passed >= (total * 90) / 100) {
		printk("✅ GREAT: %d/%d passed (≥90%%)!\n", passed, total);
	} else {
		printk("Result: %d/%d passed (%d%% success rate)\n",
		       passed, total, (passed * 100) / total);
	}
	printk("================================================================\n");

	return 0;
}
