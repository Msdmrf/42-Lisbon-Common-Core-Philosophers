/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migusant <migusant@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 09:24:43 by migusant          #+#    #+#             */
/*   Updated: 2026/03/05 13:25:15 by migusant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

pthread_mutex_t	*init_forks(int count)
{
	pthread_mutex_t	*forks;
	int				i;

	forks = malloc(sizeof(pthread_mutex_t) * count);
	if (!forks)
		return (NULL);
	i = 0;
	while (i < count)
	{
		if (pthread_mutex_init(&forks[i], NULL) != 0)
		{
			while (--i >= 0)
				pthread_mutex_destroy(&forks[i]);
			free(forks);
			return (NULL);
		}
		i++;
	}
	return (forks);
}

static void	cleanup_partial_philos(t_philo *philos, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		pthread_mutex_destroy(&philos[i].meal_mutex);
		i++;
	}
	free(philos);
}

t_philo	*init_philos(t_data *data, pthread_mutex_t *forks)
{
	t_philo	*philos;
	int		i;

	philos = malloc(sizeof(t_philo) * data->philo_count);
	if (!philos)
		return (NULL);
	i = 0;
	while (i < data->philo_count)
	{
		philos[i].id = i + 1;
		philos[i].meals_eaten = 0;
		philos[i].last_meal_time = 0;
		philos[i].data = data;
		philos[i].left_fork = &forks[i];
		philos[i].right_fork = &forks[(i + 1) % data->philo_count];
		if (pthread_mutex_init(&philos[i].meal_mutex, NULL) != 0)
		{
			cleanup_partial_philos(philos, i);
			return (NULL);
		}
		i++;
	}
	return (philos);
}

void	destroy_mutexes(void)
{
	int	i;

	if (!singleton()->philos || !singleton()->forks)
		return ;
	i = 0;
	while (i < singleton()->data->philo_count)
	{
		pthread_mutex_destroy(&singleton()->philos[i].meal_mutex);
		pthread_mutex_destroy(&singleton()->forks[i]);
		i++;
	}
}
