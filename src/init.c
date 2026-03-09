/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migusant <migusant@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 09:24:43 by migusant          #+#    #+#             */
/*   Updated: 2026/03/09 16:47:45 by migusant         ###   ########.fr       */
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

// Buffer - Worst Offset + Stagger + Contention + Extra Safety
// 0ms
// 1ms (even) +  1.25ms (stagger) +  5ms (contention) + 4.75ms (safety) = 12ms
// 1ms (even) +     5ms (stagger) + 10ms (contention) +    5ms (safety) = 21ms
// 1ms (even) + 11.25ms (stagger) + 15ms (contention) + 4.75ms (safety) = 32ms
// 1ms (even) +    20ms (stagger) + 20ms (contention) +    5ms (safety) = 46ms
void	init_meal_times(void)
{
	long	start_time;
	long	stag_cont_buffer;
	int		i;

	start_time = singleton()->data->start_time;
	if (singleton()->data->philo_count == 1)
		stag_cont_buffer = 0;
	else if (singleton()->data->philo_count <= 50)
		stag_cont_buffer = 12;
	else if (singleton()->data->philo_count <= 100)
		stag_cont_buffer = 21;
	else if (singleton()->data->philo_count <= 150)
		stag_cont_buffer = 32;
	else
		stag_cont_buffer = 46;
	i = 0;
	while (i < singleton()->data->philo_count)
	{
		singleton()->philos[i].time_to_live = start_time + stag_cont_buffer
		+ singleton()->data->time_to_die;
		i++;
	}
}
