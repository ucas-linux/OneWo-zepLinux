extern void printk(const char *message);
extern void k_msleep(int milliseconds);

void main(void)
{
    printk("Hello from ANL module!\n");
    k_msleep(100);
}
