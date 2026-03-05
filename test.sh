# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    test.sh                                            :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: migusant <migusant@student.42lisboa.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/02/09 09:42:47 by migusant          #+#    #+#              #
#    Updated: 2026/03/05 13:29:40 by migusant         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#!/bin/bash

./philo 5 800 200 200 &
PID=$!
echo "Started philo with PID $PID at $(date +%H:%M:%S)"

# Check every second for 65 seconds
for ((i=1; i<=65; i++)); do
    if !ps -p $PID > /dev/null 2>&1; then
        echo "Philo died at second $i! ($(date +%H:%M:%S))"
        exit 1
    fi
    echo "Second $i: philo still alive"
    sleep 1
done

echo "Philo survived 65 seconds! ($(date +%H:%M:%S))"
kill $PID
