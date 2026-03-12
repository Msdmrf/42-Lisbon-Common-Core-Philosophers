/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migusant <migusant@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 15:31:30 by migusant          #+#    #+#             */
/*   Updated: 2026/03/12 13:47:34 by migusant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

static void	philo_eat(t_philo *philo)
{
	long	current_time;

	current_time = get_time_ms();
	philo->last_meal_time = current_time;
	print_status(philo, "is eating");
	philo->meals_eaten++;
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

static void	init_philosopher(t_philo *philo, t_data *data, int id)
{
	philo->id = id;
	philo->pid = getpid();
	philo->meals_eaten = 0;
	philo->last_meal_time = data->start_time;
	philo->data = data;
	pthread_create(&philo->monitor_thread, NULL, monitor_routine, philo);
	pthread_detach(philo->monitor_thread);
}

void	philo_process(t_data *data, int id)
{
	t_philo	philo;

	init_philosopher(&philo, data, id);
	if (id % 2 == 0)
		usleep(1000);
	while (!is_simulation_stopped(data))
	{
		if (take_forks(&philo))
			break ;
		philo_eat(&philo);
		release_forks(&philo);
		if (data->must_eat_count != -1
			&& philo.meals_eaten >= data->must_eat_count)
		{
			cleanup_resources(CLEANUP_CHILD);
			exit(0);
		}
		philo_sleep(&philo);
		philo_think(&philo);
	}
	cleanup_resources(CLEANUP_CHILD);
	exit(0);
}
