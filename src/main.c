/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migusant <migusant@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 10:21:15 by migusant          #+#    #+#             */
/*   Updated: 2026/03/05 13:21:23 by migusant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

void	start_threads(void)
{
	int	i;

	i = 0;
	while (i < singleton()->data->philo_count)
	{
		if (pthread_create(&singleton()->philos[i].thread, NULL,
				philo_routine, &singleton()->philos[i]) != 0)
		{
			stop_simulation();
			return ;
		}
		i++;
	}
}

void	join_threads(void)
{
	int	i;

	i = 0;
	while (i < singleton()->data->philo_count)
	{
		pthread_join(singleton()->philos[i].thread, NULL);
		i++;
	}
}

void	free_resources(void)
{
	if (singleton()->data)
		free(singleton()->data);
	if (singleton()->philos)
		free(singleton()->philos);
	if (singleton()->forks)
		free(singleton()->forks);
}

void	cleanup_and_exit(void)
{
	if (singleton()->data)
	{
		pthread_mutex_destroy(&singleton()->data->stop_mutex);
		pthread_mutex_destroy(&singleton()->data->print_mutex);
	}
	if (singleton()->philos && singleton()->data)
		destroy_mutexes();
	reset_signals();
	free_resources();
	singleton()->data = NULL;
	singleton()->philos = NULL;
	singleton()->forks = NULL;
}

static void	init_singleton(t_data *data, t_philo *philos,
		pthread_mutex_t *forks)
{
	singleton()->data = data;
	singleton()->philos = philos;
	singleton()->forks = forks;
}

static void	init_meal_times(void)
{
	int		i;
	long	start_time;

	start_time = singleton()->data->start_time;
	i = 0;
	while (i < singleton()->data->philo_count)
	{
		singleton()->philos[i].last_meal_time = start_time;
		i++;
	}
}

int	main(int argc, char **argv)
{
	t_data			*data;
	pthread_mutex_t	*forks;
	t_philo			*philos;

	data = parse_args(argc, argv);
	if (!data)
		return (1);
	forks = init_forks(data->philo_count);
	philos = init_philos(data, forks);
	if (!forks || !philos)
	{
		init_singleton(data, philos, forks);
		cleanup_and_exit();
		return (1);
	}
	init_singleton(data, philos, forks);
	setup_signals();
	singleton()->data->start_time = get_time_ms();
	init_meal_times();
	start_threads();
	monitor_simulation();
	join_threads();
	cleanup_and_exit();
	return (0);
}
