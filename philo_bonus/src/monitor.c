/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migusant <migusant@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 15:31:40 by migusant          #+#    #+#             */
/*   Updated: 2026/03/13 13:44:09 by migusant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

bool	is_simulation_stopped(t_data *data)
{
	bool	stopped;

	sem_wait(data->stop_sem);
	stopped = data->simulation_stop;
	sem_post(data->stop_sem);
	return (stopped);
}

void	stop_simulation(t_data *data)
{
	sem_wait(data->stop_sem);
	data->simulation_stop = true;
	sem_post(data->stop_sem);
}

void	*monitor_routine(void *arg)
{
	t_philo	*philo;
	long	time_since_meal;

	philo = (t_philo *)arg;
	while (!is_simulation_stopped(philo->data) && !philo->monitor_should_stop)
	{
		usleep(500);
		time_since_meal = get_time_ms() - philo->last_meal_time;
		if (time_since_meal >= philo->data->time_to_die)
		{
			if (sem_trywait(philo->data->death_sem) == 0)
			{
				philo->died = true;
				stop_simulation(philo->data);
				sem_wait(philo->data->print_sem);
				printf("%ld %d died\n",
					get_elapsed_time(philo->data->start_time), philo->id);
				sem_post(philo->data->print_sem);
				if (PHILO_DEBUG)
					print_meal_summary("Failed");
				return (NULL);
			}
			return (NULL);
		}
	}
	return (NULL);
}
