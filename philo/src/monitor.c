/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migusant <migusant@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 10:45:47 by migusant          #+#    #+#             */
/*   Updated: 2026/03/15 19:01:00 by migusant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

bool	is_sim_stopped(void)
{
	bool	stopped;

	pthread_mutex_lock(&singleton()->data->sim_stop_mutex);
	stopped = singleton()->data->sim_stop;
	pthread_mutex_unlock(&singleton()->data->sim_stop_mutex);
	return (stopped);
}

void	sim_stop(void)
{
	pthread_mutex_lock(&singleton()->data->sim_stop_mutex);
	singleton()->data->sim_stop = true;
	pthread_mutex_unlock(&singleton()->data->sim_stop_mutex);
}

static bool	check_deaths(void)
{
	int		i;
	long	current_time;

	current_time = get_time_ms();
	i = 0;
	while (i < singleton()->data->philo_count)
	{
		pthread_mutex_lock(&singleton()->philos[i].state_mutex);
		if (current_time >= singleton()->philos[i].time_to_live)
		{
			pthread_mutex_lock(&singleton()->data->print_mutex);
			sim_stop();
			printf("%ld %d died\n",
				get_elapsed_time(singleton()->data->start_time),
				singleton()->philos[i].id);
			pthread_mutex_unlock(&singleton()->data->print_mutex);
			pthread_mutex_unlock(&singleton()->philos[i].state_mutex);
			if (PHILO_DEBUG)
				print_meal_summary("Failed");
			return (true);
		}
		pthread_mutex_unlock(&singleton()->philos[i].state_mutex);
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
		pthread_mutex_lock(&singleton()->philos[i].state_mutex);
		if (singleton()->philos[i].meals_eaten
			>= singleton()->data->must_eat_count)
			satisfied_count++;
		pthread_mutex_unlock(&singleton()->philos[i].state_mutex);
		i++;
	}
	if (satisfied_count == singleton()->data->philo_count)
	{
		sim_stop();
		if (PHILO_DEBUG)
			print_meal_summary("Completed");
		return (true);
	}
	return (false);
}

void	monitor_simulation(void)
{
	while (!is_sim_stopped())
	{
		if (check_deaths())
			break ;
		if (check_all_ate())
			break ;
		usleep(500);
	}
}
