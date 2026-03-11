/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migusant <migusant@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 10:45:26 by migusant          #+#    #+#             */
/*   Updated: 2026/03/11 10:39:50 by migusant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

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

static void	philo_eat(t_philo *philo)
{
	print_status(philo, "is eating");
	pthread_mutex_lock(&philo->meal_mutex);
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->meal_mutex);
	precise_sleep(philo->data->time_to_eat);
}

static void	philo_sleep(t_philo *philo)
{
	print_status(philo, "is sleeping");
	precise_sleep(philo->data->time_to_sleep);
}

static void	philo_think(t_philo *philo)
{
	long	think_time;

	print_status(philo, "is thinking");
	if (philo->data->philo_count % 2 == 1)
	{
		think_time = philo->data->time_to_eat * 2
			- philo->data->time_to_sleep;
		if (think_time > 0 && think_time < philo->data->time_to_die)
			usleep(think_time * 1000);
	}
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;
	long	stagger_ms;

	philo = (t_philo *)arg;
	stagger_ms = calculate_stagger_delay(philo);
	if (stagger_ms > 0)
		usleep(stagger_ms);
	while (!is_simulation_stopped())
	{
		if (take_forks(philo))
			break ;
		philo_eat(philo);
		release_forks(philo);
		philo_sleep(philo);
		philo_think(philo);
	}
	return (NULL);
}
