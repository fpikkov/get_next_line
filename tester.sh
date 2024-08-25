#!/bin/bash

# Variables
#MEM_CHK=$(valgrind --leak-check=full --show-leak-kinds=all -s)
GRN='\033[0;32m'
NC='\033[0m'
RED='\033[0;31m'

# Compile routine
compile_prog () {
	if [ $# -eq 1 ]; then
		cc -Wall -Wextra -Werror get_next_line.c get_next_line_utils.c testing.c -D BUFFER_SIZE=$1
	else
		cc -Wall -Wextra -Werror get_next_line.c get_next_line_utils.c testing.c
	fi
}

# Test routine
run_tests () {
printf "\n${GRN}Fresh.txt${NC}\n"
./a.out

printf "\n${GRN}One char + newline${NC}\n"
./a.out tests/1_nl.txt

printf "\n${GRN}One char + no newline${NC}\n"
./a.out tests/1_no_nl.txt

printf "\n${GRN}Ten chars + newline${NC}\n"
./a.out tests/10_nl.txt

printf "\n${GRN}Ten chars + no newline${NC}\n"
./a.out tests/10_no_nl.txt

printf "\n${GRN}Multiple newlines${NC}\n"
./a.out tests/multiple_nl.txt

printf "\n${GRN}Multiple nls + no newline${NC}\n"
./a.out tests/multiple_no_nl.txt
}


# Default BUFFER_SIZE
compile_prog
printf "\n${RED}Using default BUFFER_SIZE${NC}\n"
run_tests

# BUFFER_SIZE of 1
compile_prog 1
printf "\n${RED}BUFFER_SIZE = 1${NC}\n"
run_tests

# BUFFER_SIZE of 10
compile_prog 10
printf "\n${RED}BUFFER_SIZE = 10${NC}\n"
run_tests

# BUFFER_SIZE of 42
compile_prog 42
printf "\n${RED}BUFFER_SIZE = 42${NC}\n"
run_tests

# BUFFER_SIZE of 10 000
compile_prog 10000
printf "\n${RED}BUFFER_SIZE = 10 000${NC}\n"
run_tests
