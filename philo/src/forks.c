/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migusant <migusant@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/05 17:32:25 by migusant          #+#    #+#             */
/*   Updated: 2026/03/15 21:31:19 by migusant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

static int	handle_single_fork(t_philo *philo)
{
	pthread_mutex_lock(philo->right_fork_mutex);
	print_status(philo, "has taken a fork");
	pthread_mutex_unlock(philo->right_fork_mutex);
	precise_sleep(philo->data->time_to_die);
	return (1);
}

static int	take_forks_even(t_philo *philo)
{
	pthread_mutex_lock(philo->right_fork_mutex);
	if (is_sim_stopped())
	{
		pthread_mutex_unlock(philo->right_fork_mutex);
		return (1);
	}
	print_status(philo, "has taken a fork");
	pthread_mutex_lock(philo->left_fork_mutex);
	if (is_sim_stopped())
	{
		pthread_mutex_unlock(philo->right_fork_mutex);
		pthread_mutex_unlock(philo->left_fork_mutex);
		return (1);
	}
	print_status(philo, "has taken a fork");
	return (0);
}

static int	take_forks_odd(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork_mutex);
	if (is_sim_stopped())
	{
		pthread_mutex_unlock(philo->left_fork_mutex);
		return (1);
	}
	print_status(philo, "has taken a fork");
	pthread_mutex_lock(philo->right_fork_mutex);
	if (is_sim_stopped())
	{
		pthread_mutex_unlock(philo->left_fork_mutex);
		pthread_mutex_unlock(philo->right_fork_mutex);
		return (1);
	}
	print_status(philo, "has taken a fork");
	return (0);
}

int	take_forks(t_philo *philo)
{
	if (philo->data->philo_count == 1)
		return (handle_single_fork(philo));
	if (philo->id % 2 == 0)
	{
		if (take_forks_even(philo))
			return (1);
	}
	else
	{
		if (take_forks_odd(philo))
			return (1);
	}
	pthread_mutex_lock(&philo->state_mutex);
	philo->time_to_live = get_time_ms() + philo->data->time_to_die;
	pthread_mutex_unlock(&philo->state_mutex);
	return (0);
}

void	release_forks(t_philo *philo)
{
	if (philo->data->philo_count == 1)
		return ;
	pthread_mutex_unlock(philo->right_fork_mutex);
	pthread_mutex_unlock(philo->left_fork_mutex);
}
