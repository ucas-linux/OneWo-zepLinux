#!/bin/bash
# AS32x601 一键编译脚本
# 使用方法: ./build_as32x601.sh [clean|flash|test]

set -e

PROJECT_DIR="/opt/Program/UCAS/OneWo-zepLinux"
BUILD_DIR="build-as32-anl"
TARGET="as32x601_evb/as32x601"
SAMPLE="zephyr/samples/ansilic/as32x601_shell_process"

cd "$PROJECT_DIR"

# 颜色定义
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
NC='\033[0m' # No Color

echo -e "${GREEN}======================================${NC}"
echo -e "${GREEN}  AS32x601 编译脚本${NC}"
echo -e "${GREEN}======================================${NC}"

case "$1" in
    clean)
        echo -e "${YELLOW}🧹 清理编译目录...${NC}"
        west build -p always -b "$TARGET" -d "$BUILD_DIR" "$SAMPLE"
        ;;
    flash)
        echo -e "${YELLOW}🔥 烧录固件...${NC}"
        if [ ! -d "$BUILD_DIR" ]; then
            echo -e "${RED}❌ 编译目录不存在，请先编译！${NC}"
            exit 1
        fi
        west flash -d "$BUILD_DIR" --runner jlink
        echo -e "${GREEN}✅ 烧录完成！${NC}"
        echo -e "${YELLOW}💡 打开串口: screen /dev/ttyUSB0 115200${NC}"
        ;;
    test)
        echo -e "${YELLOW}🧪 编译测试版本...${NC}"
        west build -p always -b qemu_cortex_m3 -d build-test-phase1 \
            zephyr/tests/kernel/process
        echo -e "${GREEN}✅ 测试版本编译完成！${NC}"
        echo -e "${YELLOW}💡 运行测试: west build -d build-test-phase1 -t run${NC}"
        ;;
    incremental|"")
        echo -e "${YELLOW}📦 增量编译...${NC}"
        west build -b "$TARGET" -d "$BUILD_DIR" "$SAMPLE"
        ;;
    *)
        echo "用法: $0 {clean|flash|test|incremental}"
        echo ""
        echo "  clean       - 完全清理并重新编译"
        echo "  flash       - 烧录固件到板子"
        echo "  test        - 编译测试版本"
        echo "  incremental - 增量编译（默认）"
        exit 1
        ;;
esac

# 检查编译结果
if [ $? -eq 0 ]; then
    echo ""
    echo -e "${GREEN}======================================${NC}"
    echo -e "${GREEN}✅ 操作成功完成！${NC}"
    echo -e "${GREEN}======================================${NC}"

    if [ "$1" != "flash" ] && [ "$1" != "test" ]; then
        echo ""
        echo -e "${YELLOW}📊 内存使用情况:${NC}"
        tail -5 "$BUILD_DIR/zephyr/zephyr.stat" 2>/dev/null || echo "无统计信息"
        echo ""
        echo -e "${YELLOW}下一步操作:${NC}"
        echo "  烧录: ./build_as32x601.sh flash"
        echo "  或手动: west flash -d $BUILD_DIR --runner jlink"
    fi
else
    echo -e "${RED}❌ 操作失败！${NC}"
    exit 1
fi
