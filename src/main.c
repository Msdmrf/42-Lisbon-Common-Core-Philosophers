/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migusant <migusant@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 10:21:15 by migusant          #+#    #+#             */
/*   Updated: 2026/03/08 12:54:59 by migusant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

int	main(int argc, char **argv)
{
	t_data			*data;
	pthread_mutex_t	*fork_mutexes;
	t_philo			*philos;

	data = parse_args(argc, argv);
	if (!data)
		return (1);
	fork_mutexes = init_fork_mutexes(data->philo_count);
	philos = init_philos(data, fork_mutexes);
	singleton()->data = data;
	singleton()->fork_mutexes = fork_mutexes;
	singleton()->philos = philos;
	setup_signals(SIG_SETUP);
	if (!fork_mutexes || !philos)
		return (cleanup_and_exit(), 1);
	singleton()->data->start_time = get_time_ms();
	init_meal_times();
	start_threads();
	monitor_simulation();
	join_threads();
	cleanup_and_exit();
	return (0);
}
