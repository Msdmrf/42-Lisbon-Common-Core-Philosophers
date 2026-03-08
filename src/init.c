/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migusant <migusant@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 09:24:43 by migusant          #+#    #+#             */
/*   Updated: 2026/03/08 23:08:07 by migusant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

pthread_mutex_t	*init_fork_mutexes(int count)
{
	pthread_mutex_t	*fork_mutexes;
	int				i;

	fork_mutexes = malloc(sizeof(pthread_mutex_t) * count);
	if (!fork_mutexes)
		return (NULL);
	i = 0;
	while (i < count)
	{
		if (pthread_mutex_init(&fork_mutexes[i], NULL) != 0)
		{
			while (--i >= 0)
				pthread_mutex_destroy(&fork_mutexes[i]);
			free(fork_mutexes);
			return (NULL);
		}
		i++;
	}
	return (fork_mutexes);
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

t_philo	*init_philos(t_data *data, pthread_mutex_t *fork_mutexes)
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
		philos[i].time_to_live = 0;
		philos[i].data = data;
		philos[i].right_fork_mutex = &fork_mutexes[(i + 1) % data->philo_count];
		philos[i].left_fork_mutex = &fork_mutexes[i];
		if (pthread_mutex_init(&philos[i].meal_mutex, NULL) != 0)
		{
			cleanup_partial_philos(philos, i);
			return (NULL);
		}
		i++;
	}
	return (philos);
}

void	init_meal_times(void)
{
	int		i;
	long	start_time;

	start_time = singleton()->data->start_time;
	i = 0;
	while (i < singleton()->data->philo_count)
	{
		singleton()->philos[i].time_to_live = start_time
		+ singleton()->data->time_to_die;
		i++;
	}
}
