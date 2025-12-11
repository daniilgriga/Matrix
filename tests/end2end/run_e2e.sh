#!/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

if [ $# -eq 0 ]; then
    echo -e "${RED}ERROR: Please provide path to binary${NC}"
    echo "Usage: $0 <path_to_binary>"
    echo "Example: $0 ../../driver"
    exit 1
fi

PROGRAM_BIN="$1"
PASSED=0
FAILED=0

if [ ! -f "$PROGRAM_BIN" ]; then
    echo -e "${RED}ERROR: $PROGRAM_BIN not found!${NC}"
    exit 1
fi

echo "Running end-to-end tests for Matrix Determinant"
echo "========================================"

for dat_file in *.dat; do
    test_id="${dat_file%.dat}"
    ans_file="${test_id}.ans"

    if [ ! -f "$ans_file" ]; then
        echo -e "${YELLOW}WARNING: $ans_file not found, skipping...${NC}"
        continue
    fi

    actual_output=$("$PROGRAM_BIN" < "$dat_file" 2>/dev/null)
    expected_output=$(cat "$ans_file")

    if [ "$actual_output" == "$expected_output" ]; then
        echo -e "Test $test_id: ${GREEN}[PASSED]${NC}"
        ((PASSED++))
    else
        echo -e "Test $test_id: ${RED}[FAILED]${NC}"
        echo "  Expected: $expected_output"
        echo "  Got:      $actual_output"
        ((FAILED++))
    fi
done

echo "========================================"
echo "Results: $PASSED passed, $FAILED failed"

if [ $FAILED -gt 0 ]; then
    exit 1
else
    echo -e "${GREEN}All tests passed!${NC}"
    exit 0
fi
