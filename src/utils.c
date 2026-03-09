/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migusant <migusant@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 09:06:17 by migusant          #+#    #+#             */
/*   Updated: 2026/03/09 10:16:58 by migusant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

long	get_elapsed_time(long start_time)
{
	return (get_time_ms() - start_time);
}

void	print_status(t_philo *philo, char *status)
{
	long	timestamp;

	if (is_simulation_stopped())
		return ;
	pthread_mutex_lock(&philo->data->print_mutex);
	if (!is_simulation_stopped())
	{
		timestamp = get_elapsed_time(philo->data->start_time);
		printf("%ld %d %s\n", timestamp, philo->id, status);
	}
	pthread_mutex_unlock(&philo->data->print_mutex);
}

void	precise_sleep(long milliseconds)
{
	long	start;
	long	elapsed;

	start = get_time_ms();
	while (1)
	{
		elapsed = get_time_ms() - start;
		if (elapsed >= milliseconds)
			break ;
		usleep(500);
	}
}

void	print_meal_summary(char *message)
{
	int	i;

	pthread_mutex_lock(&singleton()->data->print_mutex);
	printf("\n=== Simulation %s ===\n", message);
	i = 0;
	while (i < singleton()->data->philo_count)
	{
		pthread_mutex_lock(&singleton()->philos[i].meal_mutex);
		printf("Philosopher %d ate %d times\n",
			singleton()->philos[i].id,
			singleton()->philos[i].meals_eaten);
		pthread_mutex_unlock(&singleton()->philos[i].meal_mutex);
		i++;
	}
	pthread_mutex_unlock(&singleton()->data->print_mutex);
}
