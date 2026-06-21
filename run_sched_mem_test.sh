#!/bin/bash

# AS32X601 调度和内存测试运行脚本

BOARD="as32x601_evb/as32x601"

echo "========================================"
echo "AS32X601 Scheduler & Memory Test Suite"
echo "========================================"
echo ""

# 检查是否在正确的目录
if [ ! -d "zephyr" ]; then
    echo "错误: 请在项目根目录运行此脚本"
    exit 1
fi

# 函数: 构建和运行测试
run_test() {
    local test_name=$1
    local test_path=$2

    echo ""
    echo "========================================"
    echo "测试: $test_name"
    echo "========================================"

    # 构建
    echo "正在构建..."
    west build -p always -b $BOARD $test_path

    if [ $? -ne 0 ]; then
        echo "❌ 构建失败: $test_name"
        return 1
    fi

    echo "✅ 构建成功: $test_name"

    # 提示用户烧录和运行
    echo ""
    echo "请执行以下操作:"
    echo "  1. 烧录到板子: west flash"
    echo "  2. 打开串口监控查看测试结果"
    echo ""
    read -p "按回车继续下一个测试..."

    return 0
}

# 选择测试
echo "请选择要运行的测试:"
echo "  1) 调度器测试 (Scheduler Test)"
echo "  2) 内存管理测试 (Memory Test)"
echo "  3) 运行所有测试"
echo "  q) 退出"
echo ""
read -p "请输入选择 [1-3/q]: " choice

case $choice in
    1)
        run_test "调度器测试" "zephyr/samples/ansilic/as32x601_scheduler_test"
        ;;
    2)
        run_test "内存管理测试" "zephyr/samples/ansilic/as32x601_memory_test"
        ;;
    3)
        run_test "调度器测试" "zephyr/samples/ansilic/as32x601_scheduler_test"
        run_test "内存管理测试" "zephyr/samples/ansilic/as32x601_memory_test"
        ;;
    q|Q)
        echo "退出"
        exit 0
        ;;
    *)
        echo "无效选择"
        exit 1
        ;;
esac

echo ""
echo "========================================"
echo "测试运行完成！"
echo "========================================"
