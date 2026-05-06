#!/bin/bash
# LMZ Code Formatting Script
#
# Formats C code using clang-format
#
# Usage: ./format.sh [file_or_directory]

set -e

# Colors
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

# Default: format all code
TARGET=${1:-"code/"}

echo -e "${GREEN}=== LMZ Format Script ===${NC}"
echo "Target: ${TARGET}"
echo ""

# Check for clang-format
if ! command -v clang-format &> /dev/null; then
    echo -e "${YELLOW}clang-format not found. Installing...${NC}"
    # Install instructions would go here
    echo "Please install clang-format:"
    echo "  Ubuntu: sudo apt install clang-format"
    echo "  macOS: brew install clang-format"
    exit 1
fi

# Format files
echo -e "${YELLOW}Formatting files...${NC}"

if [ -f "${TARGET}" ]; then
    # Single file
    clang-format -i -style=file "${TARGET}"
    echo -e "${GREEN}Formatted: ${TARGET}${NC}"
else
    # Directory
    find "${TARGET}" -type f \( -name "*.c" -o -name "*.h" \) -exec clang-format -i -style=file {} \;
    echo -e "${GREEN}Formatted all C/C++ files in ${TARGET}${NC}"
fi

echo ""
echo -e "${GREEN}Formatting complete!${NC}"
