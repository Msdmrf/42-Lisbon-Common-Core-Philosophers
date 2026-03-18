/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migusant <migusant@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 09:17:36 by migusant          #+#    #+#             */
/*   Updated: 2026/03/18 12:19:56 by migusant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo_bonus.h"

void	philo_eat(t_philo *philo)
{
	print_status(philo, "is eating");
	philo->meals_eaten++;
	precise_sleep(philo->data->time_to_eat);
}

void	philo_sleep(t_philo *philo)
{
	print_status(philo, "is sleeping");
	precise_sleep(philo->data->time_to_sleep);
}

void	philo_think(t_philo *philo)
{
	long	think_time;

	print_status(philo, "is thinking");
	if (philo->data->philo_count % 2 == 1)
	{
		think_time = philo->data->time_to_eat * 2
			- philo->data->time_to_sleep;
		if (think_time > 0 && think_time < philo->data->time_to_die)
			precise_sleep(think_time);
	}
}

void	philo_cycle(t_philo *philo)
{
	if (take_forks(philo))
		return ;
	philo_eat(philo);
	release_forks(philo);
	philo_sleep(philo);
	philo_think(philo);
}
