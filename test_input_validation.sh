# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    test_input_validation.sh                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: migusant <migusant@student.42lisboa.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/03/06 21:22:26 by migusant          #+#    #+#              #
#    Updated: 2026/03/08 11:01:15 by migusant         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#!/bin/bash

echo "=== Testing Input Validation ==="
echo ""

make re

echo "Test 1: Valid input (should work)"
./philo 5 800 200 200 5
echo "Exit code: $?"
echo ""

echo "Test 2: Non-numeric input"
./philo abc 800 200 200 5
echo "Exit code: $?"
echo ""

echo "Test 3.1: Mixed valid/invalid"
./philo abc5 800 200 200 5
echo "Exit code: $?"
echo ""

echo "Test 3.2: Mixed valid/invalid"
./philo 5abc 800 200 200 5
echo "Exit code: $?"
echo ""

echo "Test 3.3: Mixed valid/invalid"
./philo ab5c 800 200 200 5
echo "Exit code: $?"
echo ""

echo "Test 4: Trailing garbage"
./philo "5   " 800 200 200 5
echo "Exit code: $?"
echo ""

echo "Test 5: Integer overflow"
./philo 999999999999999999999 800 200 200 5
echo "Exit code: $?"
echo ""

echo "Test 6.1: Negative number (should be caught by validation)"
./philo -5 800 200 200 5
echo "Exit code: $?"
echo ""

echo "Test 6.2: Negative number (should be caught by validation)"
./philo 5 -800 200 200 5
echo "Exit code: $?"
echo ""

echo "Test 6.3: Negative number (should be caught by validation)"
./philo 5 800 -200 200 5
echo "Exit code: $?"
echo ""

echo "Test 6.4: Negative number (should be caught by validation)"
./philo 5 800 200 -200 5
echo "Exit code: $?"
echo ""

echo "Test 6.5: Negative number (should be caught by validation)"
./philo 5 800 200 200 -5
echo "Exit code: $?"
echo ""

echo "Test 7: Leading zeros"
./philo 005 800 200 200 5
echo "Exit code: $?"
echo ""

echo "Test 8: Plus sign"
./philo +5 800 200 200 5
echo "Exit code: $?"
echo ""

echo "Test 9: Empty string"
./philo "" 800 200 200 5 2>/dev/null
echo "Exit code: $?"
echo ""

echo "Test 10: Just whitespace"
./philo "   " 800 200 200 5
echo "Exit code: $?"