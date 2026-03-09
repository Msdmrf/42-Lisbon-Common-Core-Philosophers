/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migusant <migusant@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 10:45:47 by migusant          #+#    #+#             */
/*   Updated: 2026/03/09 10:30:47 by migusant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

bool	is_simulation_stopped(void)
{
	bool	stopped;

	pthread_mutex_lock(&singleton()->data->stop_mutex);
	stopped = singleton()->data->simulation_stop;
	pthread_mutex_unlock(&singleton()->data->stop_mutex);
	return (stopped);
}

void	stop_simulation(void)
{
	pthread_mutex_lock(&singleton()->data->stop_mutex);
	singleton()->data->simulation_stop = true;
	pthread_mutex_unlock(&singleton()->data->stop_mutex);
}

static bool	check_deaths(void)
{
	int		i;
	long	current_time;

	current_time = get_time_ms();
	i = 0;
	while (i < singleton()->data->philo_count)
	{
		pthread_mutex_lock(&singleton()->philos[i].meal_mutex);
		if (current_time >= singleton()->philos[i].time_to_live)
		{
			pthread_mutex_unlock(&singleton()->philos[i].meal_mutex);
			stop_simulation();
			pthread_mutex_lock(&singleton()->data->print_mutex);
			printf("%ld %d died\n",
				get_elapsed_time(singleton()->data->start_time),
				singleton()->philos[i].id);
			pthread_mutex_unlock(&singleton()->data->print_mutex);
			if (PHILO_DEBUG)
				print_meal_summary("Failed");
			return (true);
		}
		pthread_mutex_unlock(&singleton()->philos[i].meal_mutex);
		i++;
	}
	return (false);
}

static bool	check_all_ate(void)
{
	int	i;
	int	satisfied_count;

	if (singleton()->data->must_eat_count == -1)
		return (false);
	satisfied_count = 0;
	i = 0;
	while (i < singleton()->data->philo_count)
	{
		pthread_mutex_lock(&singleton()->philos[i].meal_mutex);
		if (singleton()->philos[i].meals_eaten
			>= singleton()->data->must_eat_count)
			satisfied_count++;
		pthread_mutex_unlock(&singleton()->philos[i].meal_mutex);
		i++;
	}
	if (satisfied_count == singleton()->data->philo_count)
	{
		stop_simulation();
		if (PHILO_DEBUG)
			print_meal_summary("Complete");
		return (true);
	}
	return (false);
}

void	monitor_simulation(void)
{
	while (!is_simulation_stopped())
	{
		if (check_deaths())
			break ;
		if (check_all_ate())
			break ;
		usleep(500);
	}
}
