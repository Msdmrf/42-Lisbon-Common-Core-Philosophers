# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    test_death_detection.sh                            :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: migusant <migusant@student.42lisboa.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/03/06 21:45:06 by migusant          #+#    #+#              #
#    Updated: 2026/03/08 23:05:41 by migusant         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#!/bin/bash

echo "=== Testing Death Detection Accuracy ==="
echo ""

make re
echo ""

echo "Test 1: 1 philosopher (can't eat, should die at ~800ms)"
./philo 1 800 200 200
[ $? -eq 0 ] && echo "OK - Died as expected" || echo "FAILED"
echo ""

echo "Test 2: Tight timing with completion (should survive and complete)"
timeout 3 ./philo 4 310 200 100 5
[ $? -eq 124 ] && echo "TIMEOUT" || echo "OK"
echo ""

echo "Test 3: Many philosophers with completion (should complete without hanging)"
timeout 5 ./philo 10 410 200 200 7
[ $? -eq 124 ] && echo "TIMEOUT" || echo "OK"
echo ""

echo "Test 4: Death detection (philosopher should die)"
timeout 2 ./philo 4 310 400 100
EXIT_CODE=$?
if [ $EXIT_CODE -eq 124 ]; then
    echo "TIMEOUT - Death not detected"
elif [ $EXIT_CODE -eq 0 ]; then
    echo "OK - Death detected"
else
    echo "Unexpected exit code: $EXIT_CODE"
fi
echo ""

echo "Test 5: Consistency check - run 10 times (all should behave the same)"
for i in {1..10}; do
    echo -n "Run $i: "
    timeout 3 ./philo 4 310 200 100 5 > /dev/null 2>&1
    [ $? -eq 124 ] && echo "TIMEOUT" || echo "OK"
done
echo ""

echo "Test 6: Edge case - exact death timing"
timeout 2 ./philo 2 200 200 200
[ $? -eq 124 ] && echo "TIMEOUT" || echo "OK - Death detected"