/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migusant <migusant@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 10:45:26 by migusant          #+#    #+#             */
/*   Updated: 2026/03/08 21:58:38 by migusant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

static int	handle_single_philo(t_philo *philo)
{
	pthread_mutex_lock(philo->right_fork_mutex);
	print_status(philo, "has taken a fork");
	pthread_mutex_unlock(philo->right_fork_mutex);
	return (1);
}

static int	take_forks_even(t_philo *philo)
{
	pthread_mutex_lock(philo->right_fork_mutex);
	if (is_simulation_stopped())
		return (pthread_mutex_unlock(philo->right_fork_mutex), 1);
	print_status(philo, "has taken a fork");
	pthread_mutex_lock(philo->left_fork_mutex);
	if (is_simulation_stopped())
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
	if (is_simulation_stopped())
		return (pthread_mutex_unlock(philo->left_fork_mutex), 1);
	print_status(philo, "has taken a fork");
	pthread_mutex_lock(philo->right_fork_mutex);
	if (is_simulation_stopped())
	{
		pthread_mutex_unlock(philo->left_fork_mutex);
		pthread_mutex_unlock(philo->right_fork_mutex);
		return (1);
	}
	print_status(philo, "has taken a fork");
	return (0);
}

static int	take_forks(t_philo *philo)
{
	if (philo->data->philo_count == 1)
		return (handle_single_philo(philo));
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
	pthread_mutex_lock(&philo->meal_mutex);
	philo->time_to_live = get_time_ms() + philo->data->time_to_die;
	pthread_mutex_unlock(&philo->meal_mutex);
	return (0);
}

static void	philo_eat(t_philo *philo)
{
	print_status(philo, "is eating");
	pthread_mutex_lock(&philo->meal_mutex);
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->meal_mutex);
	precise_sleep(philo->data->time_to_eat);
}

static void	release_forks(t_philo *philo)
{
	if (philo->data->philo_count == 1)
		return ;
	pthread_mutex_unlock(philo->right_fork_mutex);
	pthread_mutex_unlock(philo->left_fork_mutex);
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

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		usleep(1000);
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
