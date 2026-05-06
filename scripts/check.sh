#!/bin/bash
# LMZ Code Quality Check Script
#
# Runs various code quality checks
#
# Usage: ./check.sh

set -e

# Colors
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
NC='\033[0m'

echo -e "${GREEN}=== LMZ Code Quality Checks ===${NC}"
echo ""

# Check 1: Copyright headers
echo -e "${YELLOW}Checking copyright headers...${NC}"
COPYRIGHT_COUNT=$(grep -r "Copyright (c) 2025 LMZ Project" code/ 2>/dev/null | wc -l)
TOTAL_FILES=$(find code/ -name "*.c" -o -name "*.h" 2>/dev/null | wc -l)

if [ "${COPYRIGHT_COUNT}" -eq "${TOTAL_FILES}" ]; then
    echo -e "${GREEN}✓ All files have copyright headers${NC}"
else
    echo -e "${RED}✗ ${COPYRIGHT_COUNT}/${TOTAL_FILES} files have copyright headers${NC}"
fi
echo ""

# Check 2: Trailing whitespace
echo -e "${YELLOW}Checking for trailing whitespace...${NC}"
WHITESPACE_FILES=$(grep -rl " $" code/ 2>/dev/null | wc -l)

if [ "${WHITESPACE_FILES}" -eq 0 ]; then
    echo -e "${GREEN}✓ No trailing whitespace found${NC}"
else
    echo -e "${RED}✗ Found trailing whitespace in ${WHITESPACE_FILES} files${NC}"
    echo "  Run: find code/ -type f -exec sed -i 's/[[:space:]]*$//' {} +"
fi
echo ""

# Check 3: Line length
echo -e "${YELLOW}Checking for long lines (>80 chars)...${NC}"
LONG_LINES=$(awk 'length>80' code/src/*.c code/include/*.h 2>/dev/null | wc -l)

if [ "${LONG_LINES}" -eq 0 ]; then
    echo -e "${GREEN}✓ No long lines found${NC}"
else
    echo -e "${YELLOW}! Found ${LONG_LINES} lines longer than 80 characters${NC}"
fi
echo ""

# Check 4: TODO comments
echo -e "${YELLOW}Checking for TODO comments...${NC}"
TODO_COUNT=$(grep -r "TODO:" code/ 2>/dev/null | wc -l)

if [ "${TODO_COUNT}" -eq 0 ]; then
    echo -e "${GREEN}✓ No TODOs found${NC}"
else
    echo -e "${YELLOW}! Found ${TODO_COUNT} TODO comments${NC}"
    echo "  Review and address remaining TODOs"
fi
echo ""

# Check 5: Check return values
echo -e "${YELLOW}Checking for unchecked returns...${NC}"
# Simple check for common patterns
UNCHECKED_RETURNS=$(grep -r "pthread_.*();" code/src/*.c 2>/dev/null | wc -l)

if [ "${UNCHECKED_RETURNS}" -eq 0 ]; then
    echo -e "${GREEN}✓ All pthread calls checked${NC}"
else
    echo -e "${YELLOW}! Found ${UNCHECKED_RETURNS} potentially unchecked pthread calls${NC}"
    echo "  Review return values"
fi
echo ""

# Check 6: Header guards
echo -e "${YELLOW}Checking header guards...${NC}"
MISSING_GUARDS=0

for header in code/include/*.h; do
    if ! grep -q "#ifndef" "${header}" || ! grep -q "#endif" "${header}"; then
        MISSING_GUARDS=$((MISSING_GUARDS + 1))
    fi
done

if [ "${MISSING_GUARDS}" -eq 0 ]; then
    echo -e "${GREEN}✓ All headers have guards${NC}"
else
    echo -e "${RED}✗ ${MISSING_GUARDS} headers missing guards${NC}"
fi
echo ""

# Summary
echo -e "${GREEN}=== Check Summary ===${NC}"
echo "Copyright headers: ${COPYRIGHT_COUNT}/${TOTAL_FILES}"
echo "Files with trailing whitespace: ${WHITESPACE_FILES}"
echo "Lines > 80 chars: ${LONG_LINES}"
echo "TODO comments: ${TODO_COUNT}"
echo "Potentially unchecked returns: ${UNCHECKED_RETURNS}"
echo "Headers without guards: ${MISSING_GUARDS}"
echo ""

# Exit with appropriate code
if [ "${WHITESPACE_FILES}" -gt 0 ] || [ "${MISSING_GUARDS}" -gt 0 ]; then
    exit 1
fi

echo -e "${GREEN}All checks passed!${NC}"
