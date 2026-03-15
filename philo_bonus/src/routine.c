/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migusant <migusant@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 15:31:30 by migusant          #+#    #+#             */
/*   Updated: 2026/03/13 19:31:34 by migusant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo_bonus.h"

static bool	philo_init(t_philo *philo, t_data *data, int id)
{
	philo->id = id;
	philo->meals_eaten = 0;
	philo->last_meal_time = data->start_time;
	philo->data = data;
	philo->monitor_created = false;
	philo->monitor_should_stop = false;
	philo->died = false;
	if (pthread_create(&philo->monitor_thread, NULL,
			monitor_routine, philo) != 0)
		return (false);
	philo->monitor_created = true;
	return (true);
}

static void	stop_monitor(t_philo *philo)
{
	if (philo->monitor_created)
	{
		philo->monitor_should_stop = true;
		pthread_join(philo->monitor_thread, NULL);
	}
}

static bool	philo_should_exit(t_philo *philo)
{
	if (philo->data->must_eat_count == -1)
		return (false);
	return (philo->meals_eaten >= philo->data->must_eat_count);
}

static void	philo_cycle(t_philo *philo)
{
	if (take_forks(philo))
		return ;
	philo_eat(philo);
	release_forks(philo);
	philo_sleep(philo);
	philo_think(philo);
}

void	philo_process(t_data *data, int id)
{
	t_philo	philo;

	setup_signals(SIG_CHILD);
	if (!philo_init(&philo, data, id))
	{
		cleanup_resources(CLEANUP_CHILD);
		exit(1);
	}
	if (id % 2 == 0)
		usleep(1000);
	while (!is_simulation_stopped(data))
	{
		philo_cycle(&philo);
		if (philo_should_exit(&philo))
		{
			stop_monitor(&philo);
			cleanup_resources(CLEANUP_CHILD);
			exit(0);
		}
	}
	stop_monitor(&philo);
	cleanup_resources(CLEANUP_CHILD);
	if (philo.died)
		exit(1);
	exit(0);
}
