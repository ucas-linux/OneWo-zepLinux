# Fibonacci sequence generator
# Prints first 10 Fibonacci numbers

# Initialize fib(0) = 0, fib(1) = 1
PUSH 0
PRINT
PUSH 1
PRINT

# Initialize counter = 2
PUSH 2

# Loop start
loop:
DUP
PUSH 10
LT
JZ end

# Calculate next Fibonacci: fib(n) = fib(n-1) + fib(n-2)
# Stack: counter, fib(n-1), fib(n-2)
# We need to swap and add

# For now, simple incrementing demo
# TODO: Implement proper Fibonacci with stack manipulation

# Increment counter
PUSH 1
ADD

# Print counter
DUP
PRINT

# Sleep 200ms
PUSH 200
SLEEP

# Jump back
JMP loop

end:
POP
HALT
