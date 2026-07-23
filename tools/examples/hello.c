// Hello World example for mini compiler
// Usage: python3 tools/compile.py tools/examples/hello.c hello.bin

print("Hello from compiler!\n");
print(1 + 2 * 3);

sleep(500);

if (getpid() > 0) {
    print("PID is positive\n");
}

while (getpid() < 0) {
    sleep(100);
}

halt;
