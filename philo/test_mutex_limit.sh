# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    test_mutex_limit.sh                                :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: migusant <migusant@student.42lisboa.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/03/06 21:08:29 by migusant          #+#    #+#              #
#    Updated: 2026/03/08 11:01:11 by migusant         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#!/bin/bash

# Create artificial resource exhaustion /using strace to try and see actual pthread_mutex_init failures)
#
# What might happen:
# 	The EAGAIN errors you'll be seeing are from FUTEX_WAIT_PRIVATE, which is the underlying syscall that pthread_mutex_lock() uses.
# 
# Key observations:
# 	The errors might happen during FUTEX_WAIT (waiting on a lock), NOT during mutex initialization.
# 	In this case, the program is still running and printing ("has taken a fork", "is thinking", etc.)
# 	EAGAIN in this context means: "the mutex is locked, try again" - this is NORMAL behavior for contended mutexes, not a failure.
# 
# If this happens:
# 	futex(0x5def391322d8, FUTEX_WAIT_PRIVATE, 2, NULL) = -1 EAGAIN (Resource temporarily unavailable) 
#	(...)
# 
# EAGAIN means:
# 	"I tried to wait on this mutex, but its state changed before I could sleep."
# 	The mutex was unlocked by another thread just as this thread tried to wait.
# 	This is expected in high-contention scenarios (200 philosophers fighting for resources).

make re

echo "=== Test: Simulate mutex init failure ==="
strace -e trace=futex ./philo 200 800 200 200 2>&1 | grep -i "EAGAIN\|ENOMEM"