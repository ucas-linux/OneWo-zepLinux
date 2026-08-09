/* Minimal ANL test program for RISC-V */

extern void printk(const char *fmt);

int main(void)
{
    printk("Hello ANL!\n");
    return 0;
}
