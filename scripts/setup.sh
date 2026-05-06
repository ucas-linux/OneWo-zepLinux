#!/bin/bash
# LMZ Setup Script
#
# Sets up development environment for LMZ
#
# Usage: ./setup.sh

set -e

# Colors
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
NC='\033[0m'

echo -e "${GREEN}=== LMZ Development Setup ===${NC}"
echo ""

# Check Python 3
echo -e "${YELLOW}Checking Python 3...${NC}"
if command -v python3 &> /dev/null; then
    PYTHON_VER=$(python3 --version | cut -d' ' -f2)
    echo -e "${GREEN}✓ Python ${PYTHON_VER} found${NC}"
else
    echo -e "${RED}✗ Python 3 not found${NC}"
    exit 1
fi
echo ""

# Install Python dependencies
echo -e "${YELLOW}Installing Python dependencies...${NC}"
pip3 install --user -r requirements.txt 2>/dev/null || true
echo -e "${GREEN}✓ Python dependencies installed${NC}"
echo ""

# Make scripts executable
echo -e "${YELLOW}Making scripts executable...${NC}"
chmod +x scripts/*.sh
echo -e "${GREEN}✓ Scripts are now executable${NC}"
echo ""

# Create output directories
echo -e "${YELLOW}Creating output directories...${NC}"
mkdir -p build
mkdir -p logs
echo -e "${GREEN}✓ Output directories created${NC}"
echo ""

# Check for Zephyr environment
echo -e "${YELLOW}Checking Zephyr environment...${NC}"
if [ -n "${ZEPHYR_BASE}" ]; then
    echo -e "${GREEN}✓ ZEPHYR_BASE is set${NC}"
    echo "  ZEPHYR_BASE: ${ZEPHYR_BASE}"
else
    echo -e "${YELLOW}! ZEPHYR_BASE not set${NC}"
    echo "  Set ZEPHYR_BASE environment variable (recommended: use the local ./zephyr copy)"
    echo "  Example: export ZEPHYR_BASE=$(pwd)/zephyr"
fi
echo ""

# Check for west
echo -e "${YELLOW}Checking for west tool...${NC}"
if command -v west &> /dev/null; then
    WEST_VER=$(west --version 2>/dev/null || echo "unknown")
    echo -e "${GREEN}✓ west found (version ${WEST_VER})${NC}"
else
    echo -e "${YELLOW}! west not found${NC}"
    echo "  Install Zephyr SDK for west"
fi
echo ""

echo -e "${GREEN}=== Setup Complete ===${NC}"
echo ""
echo "Next steps:"
echo "  1. Build: ./scripts/build.sh <board>"
echo "  2. Test:  ./scripts/test.sh <board>"
echo "  3. Format: ./scripts/format.sh"
echo "  4. Check:  ./scripts/check.sh"
