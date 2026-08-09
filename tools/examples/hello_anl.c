/* Simple ANL test program for RISC-V */

// External functions from Zephyr
extern void printk(const char *fmt);
extern void k_msleep(int ms);

int main(void)
{
    printk("Hello from ANL loader!\n");
    printk("This is running on RISC-V RV32I\n");
    k_msleep(100);
    printk("ANL test completed successfully!\n");
    return 0;
}
