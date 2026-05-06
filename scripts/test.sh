#!/bin/bash
# LMZ Test Script
#
# Runs LMZ tests on specified board
#
# Usage: ./test.sh <board> [test_name]

set -e

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

# Default values
BOARD=${1:-"nucleo_f103rb"}
TEST_NAME=${2:-""}

echo -e "${GREEN}=== LMZ Test Script ===${NC}"
echo "Board: ${BOARD}"
if [ -n "${TEST_NAME}" ]; then
    echo "Test: ${TEST_NAME}"
fi
echo ""

# Check if build directory exists
if [ ! -d "build" ]; then
    echo -e "${RED}Build directory not found. Run build.sh first.${NC}"
    exit 1
fi

# Determine test path
if [ -n "${TEST_NAME}" ]; then
    TEST_PATH="tests/${TEST_NAME}"
else
    TEST_PATH="tests"
fi

echo -e "${YELLOW}Building tests...${NC}"
cd build
west build -b "${BOARD}" "../${TEST_PATH}"

if [ $? -ne 0 ]; then
    echo -e "${RED}Test build failed!${NC}"
    exit 1
fi

echo -e "${GREEN}Test build successful!${NC}"
echo ""

# Flash to board
echo -e "${YELLOW}Flashing to ${BOARD}...${NC}"
west flash

if [ $? -ne 0 ]; then
    echo -e "${RED}Flash failed!${NC}"
    exit 1
fi

echo -e "${GREEN}Flash successful!${NC}"
echo ""

# Run tests via serial
echo -e "${YELLOW}Running tests...${NC}"
echo "Connect to board serial port..."
west attach

echo ""
echo -e "${GREEN}Test run complete!${NC}"
