/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migusant <migusant@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 15:31:35 by migusant          #+#    #+#             */
/*   Updated: 2026/03/11 20:08:41 by migusant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

static int	handle_single_philo(t_philo *philo)
{
	sem_wait(philo->data->forks_sem);
	print_status(philo, "has taken a fork");
	while (!is_simulation_stopped(philo->data))
		usleep(1000);
	sem_post(philo->data->forks_sem);
	return (1);
}

int	take_forks(t_philo *philo)
{
	if (philo->data->philo_count == 1)
		return (handle_single_philo(philo));
	sem_wait(philo->data->forks_sem);
	if (is_simulation_stopped(philo->data))
	{
		sem_post(philo->data->forks_sem);
		return (1);
	}
	print_status(philo, "has taken a fork");
	sem_wait(philo->data->forks_sem);
	if (is_simulation_stopped(philo->data))
	{
		sem_post(philo->data->forks_sem);
		sem_post(philo->data->forks_sem);
		return (1);
	}
	print_status(philo, "has taken a fork");
	return (0);
}

void	release_forks(t_philo *philo)
{
	if (philo->data->philo_count == 1)
		return ;
	sem_post(philo->data->forks_sem);
	sem_post(philo->data->forks_sem);
}
