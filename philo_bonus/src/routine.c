/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migusant <migusant@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 15:31:30 by migusant          #+#    #+#             */
/*   Updated: 2026/03/18 12:26:52 by migusant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo_bonus.h"

static long	calculate_stagger_delay(t_philo *philo)
{
	long	step_ms;
	long	offset_ms;
	int		max_odd_index;

	if (philo->data->philo_count <= PHILO_COUNT_SMALL)
		step_ms = STAGGER_DELAY_SMALL;
	else if (philo->data->philo_count <= PHILO_COUNT_MEDIUM)
		step_ms = STAGGER_DELAY_MEDIUM;
	else if (philo->data->philo_count <= PHILO_COUNT_LARGE)
		step_ms = STAGGER_DELAY_LARGE;
	else
		step_ms = STAGGER_DELAY_XLARGE;
	if (philo->data->philo_count % 2 == 1)
		max_odd_index = (philo->data->philo_count - 1) / 2;
	else
		max_odd_index = (philo->data->philo_count - 2) / 2;
	if (philo->id % 2 == 1)
		offset_ms = 0;
	else
		offset_ms = (max_odd_index + 1) * step_ms;
	return (offset_ms + ((philo->id - 1) / 2) * step_ms);
}

static void	stop_monitor_thread(t_philo *philo)
{
	if (philo->monitor_created)
	{
		philo->monitor_should_stop = true;
		pthread_join(philo->monitor_thread, NULL);
	}
}

static bool	philo_should_exit(t_philo *philo)
{
	if (philo->data->must_eat_count == INT_MAX)
		return (false);
	return (philo->meals_eaten >= philo->data->must_eat_count);
}

static void	run_philo_cycle(t_philo *philo)
{
	while (!is_sim_stopped())
	{
		philo_cycle(philo);
		if (philo_should_exit(philo))
		{
			stop_monitor_thread(philo);
			cleanup_resources(CLEANUP_CHILD);
			exit(0);
		}
	}
	stop_monitor_thread(philo);
	cleanup_resources(CLEANUP_CHILD);
	if (philo->died)
		exit(1);
	exit(0);
}

void	philo_process(t_data *data, int id)
{
	t_philo	philo;
	long	stagger_ms;

	setup_signals(SIG_CHILD);
	if (!init_philo(&philo, data, id))
	{
		cleanup_resources(CLEANUP_CHILD);
		exit(1);
	}
	stagger_ms = calculate_stagger_delay(&philo);
	if (stagger_ms > 0)
		usleep(stagger_ms);
	run_philo_cycle(&philo);
}
