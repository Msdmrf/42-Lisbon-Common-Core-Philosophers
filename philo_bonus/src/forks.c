/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migusant <migusant@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 15:31:35 by migusant          #+#    #+#             */
/*   Updated: 2026/03/18 13:54:41 by migusant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo_bonus.h"

static int	handle_single_fork(t_philo *philo)
{
	sem_wait(philo->data->forks_sem);
	print_status(philo, "has taken a fork");
	sem_post(philo->data->forks_sem);
	precise_sleep(philo->data->time_to_die);
	return (1);
}

static int	wait_for_first_fork(t_philo *philo)
{
	while (sem_trywait(philo->data->forks_sem) != 0)
	{
		if (is_sim_stopped())
			return (1);
		usleep(100);
	}
	if (is_sim_stopped())
	{
		sem_post(philo->data->forks_sem);
		return (1);
	}
	print_status(philo, "has taken a fork");
	return (0);
}

static int	wait_for_second_fork(t_philo *philo)
{
	while (sem_trywait(philo->data->forks_sem) != 0)
	{
		if (is_sim_stopped())
		{
			sem_post(philo->data->forks_sem);
			return (1);
		}
		usleep(100);
	}
	if (is_sim_stopped())
	{
		sem_post(philo->data->forks_sem);
		sem_post(philo->data->forks_sem);
		return (1);
	}
	print_status(philo, "has taken a fork");
	return (0);
}

int	take_forks(t_philo *philo)
{
	if (philo->data->philo_count == 1)
		return (handle_single_fork(philo));
	if (wait_for_first_fork(philo))
		return (1);
	if (wait_for_second_fork(philo))
		return (1);
	atomic_store(&philo->time_to_live, get_time_ms() + philo->data->time_to_die);
	return (0);
}

void	release_forks(t_philo *philo)
{
	if (philo->data->philo_count == 1)
		return ;
	sem_post(philo->data->forks_sem);
	sem_post(philo->data->forks_sem);
}
