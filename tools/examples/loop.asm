# Simple loop demo
# Prints "Loop iteration: N" for N from 1 to 5

PUSH 1

loop:
# Check if counter <= 5
DUP
PUSH 6
LT
JZ end

# Print counter
DUP
PRINT

# Increment
PUSH 1
ADD

# Sleep 500ms
PUSH 500
SLEEP

# Loop back
JMP loop

end:
POP
HALT
