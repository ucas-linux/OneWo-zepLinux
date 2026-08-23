#!/usr/bin/env python3
"""Mini compiler: C-like syntax -> bytecode binary for the MCU VM."""

import struct, sys, re

OP = {
    'PUSH': 0x01, 'POP': 0x02, 'DUP': 0x03,
    'ADD': 0x10, 'SUB': 0x11, 'MUL': 0x12, 'DIV': 0x13,
    'EQ': 0x20, 'NE': 0x21, 'LT': 0x22, 'GT': 0x23,
    'JMP': 0x30, 'JZ': 0x31, 'JNZ': 0x32,
    'PRINT': 0x40, 'PRINT_STR': 0x41, 'SLEEP': 0x42, 'GETPID': 0x43,
    'HALT': 0xFF,
}

TOKEN_RE = re.compile(
    r'\s*(?:'
    r'("(?:[^"\\]|\\.)*")'
    r'|(\d+)'
    r'|([a-zA-Z_]\w*)'
    r'|([+\-*/!<>=]=?|[(){};,])'
    r')'
)

def tokenize(src):
    tokens = []
    for m in TOKEN_RE.finditer(src):
        if m.group(1):   tokens.append(('STR', m.group(1)[1:-1]))
        elif m.group(2): tokens.append(('INT', int(m.group(2))))
        elif m.group(3): tokens.append(('ID',  m.group(3)))
        elif m.group(4): tokens.append(('OP',  m.group(4)))
    return tokens

class Compiler:
    def __init__(self, tokens):
        self.tokens = tokens
        self.pos = 0
        self.code = bytearray()

    def peek(self):
        return self.tokens[self.pos] if self.pos < len(self.tokens) else None

    def consume(self, typ=None, val=None):
        t = self.tokens[self.pos]
        if typ and t[0] != typ: raise SyntaxError(f"Expected {typ}, got {t}")
        if val and t[1] != val: raise SyntaxError(f"Expected '{val}', got '{t[1]}'")
        self.pos += 1
        return t

    def emit(self, op, operand=None):
        self.code.append(OP[op])
        if operand is not None:
            self.code.extend(struct.pack('>i', operand))

    def emit_str(self, s):
        enc = s.encode('ascii')
        self.code.append(OP['PRINT_STR'])
        self.code.append(len(enc))
        self.code.extend(enc)

    def pc(self): return len(self.code)

    def patch(self, addr, target):
        struct.pack_into('>i', self.code, addr + 1, target)

    def expr(self):
        self.term()
        while self.peek() and self.peek()[0] == 'OP' and self.peek()[1] in ('+', '-', '==', '!=', '<', '>'):
            op = self.consume('OP')[1]
            self.term()
            {'+'  : lambda: self.emit('ADD'),
             '-'  : lambda: self.emit('SUB'),
             '==' : lambda: self.emit('EQ'),
             '!=' : lambda: self.emit('NE'),
             '<'  : lambda: self.emit('LT'),
             '>'  : lambda: self.emit('GT'),
            }[op]()

    def term(self):
        self.factor()
        while self.peek() and self.peek()[0] == 'OP' and self.peek()[1] in ('*', '/'):
            op = self.consume('OP')[1]
            self.factor()
            self.emit('MUL' if op == '*' else 'DIV')

    def factor(self):
        t = self.peek()
        if t is None: raise SyntaxError("Unexpected end of input")
        if t[0] == 'INT':
            self.consume(); self.emit('PUSH', t[1])
        elif t[0] == 'ID' and t[1] == 'getpid':
            self.consume()
            self.consume('OP', '('); self.consume('OP', ')')
            self.emit('GETPID')
        elif t[0] == 'OP' and t[1] == '(':
            self.consume('OP', '('); self.expr(); self.consume('OP', ')')
        else:
            raise SyntaxError(f"Unexpected token: {t}")

    def block(self):
        self.consume('OP', '{')
        while self.peek() and not (self.peek()[0] == 'OP' and self.peek()[1] == '}'):
            self.stmt()
        self.consume('OP', '}')

    def stmt(self):
        t = self.peek()
        if t is None: return
        if t[0] == 'ID' and t[1] == 'print':
            self.consume(); self.consume('OP', '(')
            if self.peek()[0] == 'STR':
                self.emit_str(self.consume()[1])
            else:
                self.expr(); self.emit('PRINT')
            self.consume('OP', ')'); self.consume('OP', ';')
        elif t[0] == 'ID' and t[1] == 'sleep':
            self.consume(); self.consume('OP', '(')
            self.expr()
            self.consume('OP', ')'); self.consume('OP', ';')
            self.emit('SLEEP')
        elif t[0] == 'ID' and t[1] == 'halt':
            self.consume(); self.consume('OP', ';'); self.emit('HALT')
        elif t[0] == 'ID' and t[1] == 'while':
            self.consume()
            loop_start = self.pc()
            self.consume('OP', '('); self.expr(); self.consume('OP', ')')
            self.emit('JZ', 0); jz_addr = self.pc() - 5
            self.block()
            self.emit('JMP', loop_start)
            self.patch(jz_addr, self.pc())
        elif t[0] == 'ID' and t[1] == 'if':
            self.consume()
            self.consume('OP', '('); self.expr(); self.consume('OP', ')')
            self.emit('JZ', 0); jz_addr = self.pc() - 5
            self.block()
            self.patch(jz_addr, self.pc())
        else:
            raise SyntaxError(f"Unknown statement: {t}")

    def compile(self):
        while self.peek():
            self.stmt()
        return bytes(self.code)

def main():
    if len(sys.argv) < 2:
        print("Usage: compile.py <input.c> [output.bin]")
        print("\nSupported syntax:")
        print("  print(\"string\");   print(expr);")
        print("  sleep(ms);         halt;")
        print("  while (expr) { }   if (expr) { }")
        print("  Expressions: integers, +,-,*,/,==,!=,<,>, getpid()")
        sys.exit(1)
    src = re.sub(r'//[^\n]*', '', open(sys.argv[1]).read())
    binary = Compiler(tokenize(src)).compile()
    out = sys.argv[2] if len(sys.argv) > 2 else sys.argv[1].rsplit('.', 1)[0] + '.bin'
    open(out, 'wb').write(binary)
    print(f"Compiled {len(binary)} bytes -> {out}")

if __name__ == '__main__':
    main()
