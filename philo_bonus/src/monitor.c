/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migusant <migusant@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 15:31:40 by migusant          #+#    #+#             */
/*   Updated: 2026/03/18 13:56:52 by migusant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo_bonus.h"

bool	is_sim_stopped(void)
{
	bool	stopped;

	sem_wait(singleton()->data->stop_sem);
	stopped = singleton()->data->sim_stop;
	sem_post(singleton()->data->stop_sem);
	return (stopped);
}

void	sim_stop(void)
{
	sem_wait(singleton()->data->stop_sem);
	singleton()->data->sim_stop = true;
	sem_post(singleton()->data->stop_sem);
}

static bool	handle_philosopher_death(t_philo *philo)
{
	if (sem_trywait(philo->data->death_sem) != 0)
		return (false);
	atomic_store(&philo->died, true);
	sim_stop();
	sem_wait(philo->data->print_sem);
	printf("%ld %d died\n",
		get_elapsed_time(philo->data->start_time), philo->id);
	sem_post(philo->data->print_sem);
	if (PHILO_DEBUG)
		printf("\n=== Simulation Failed ===\n");
	return (true);
}

void	*monitor_routine(void *arg)
{
	t_philo	*philo;
	long	current_time;
	long	deadline;

	philo = (t_philo *)arg;
	while (!is_sim_stopped() && !atomic_load(&philo->monitor_should_stop))
	{
		usleep(500);
		current_time = get_time_ms();
		deadline = atomic_load(&philo->time_to_live);
		if (current_time >= deadline)
		{
			handle_philosopher_death(philo);
			return (NULL);
		}
	}
	return (NULL);
}
