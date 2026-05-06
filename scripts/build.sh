#!/bin/bash
# LMZ Build Script
#
# Builds LMZ library for specified target board
#
# Usage: ./build.sh <board> [build_dir]

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Default values
BOARD=${1:-"nucleo_f103rb"}
BUILD_DIR=${2:-"build"}

echo -e "${GREEN}=== LMZ Build Script ===${NC}"
echo "Board: ${BOARD}"
echo "Build directory: ${BUILD_DIR}"
echo ""

# Check for required tools
echo -e "${YELLOW}Checking required tools...${NC}"

if ! command -v west &> /dev/null; then
    echo -e "${RED}Error: west not found. Install Zephyr SDK.${NC}"
    exit 1
fi

if ! command -v cmake &> /dev/null; then
    echo -e "${RED}Error: cmake not found. Install CMake.${NC}"
    exit 1
fi

if ! command -v arm-none-eabi-gcc &> /dev/null; then
    echo -e "${RED}Error: ARM GCC toolchain not found. Install Zephyr SDK.${NC}"
    exit 1
fi

echo -e "${GREEN}All required tools found.${NC}"
echo ""

# Clean previous build
if [ -d "${BUILD_DIR}" ]; then
    echo -e "${YELLOW}Cleaning previous build...${NC}"
    rm -rf "${BUILD_DIR}"
fi

# Create build directory
echo -e "${YELLOW}Creating build directory...${NC}"
mkdir -p "${BUILD_DIR}"

# Build LMZ
echo -e "${YELLOW}Building LMZ for ${BOARD}...${NC}"
cd "${BUILD_DIR}"
cmake -DBOARD="${BOARD}" ..
make -j$(nproc)

# Check build result
if [ $? -eq 0 ]; then
    echo -e "${GREEN}Build successful!${NC}"
    echo ""
    echo "Library: ${BUILD_DIR}/liblmz.a"
    echo ""
    echo -e "${YELLOW}To install or use:${NC}"
    echo "  export LD_LIBRARY_PATH=${BUILD_DIR}:\$LD_LIBRARY_PATH"
else
    echo -e "${RED}Build failed!${NC}"
    exit 1
fi
