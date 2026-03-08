/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migusant <migusant@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 09:17:31 by migusant          #+#    #+#             */
/*   Updated: 2026/03/08 12:19:05 by migusant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

static void	destroy_mutexes(void)
{
	int	i;

	if (!singleton()->fork_mutexes || !singleton()->philos)
		return ;
	i = 0;
	while (i < singleton()->data->philo_count)
	{
		pthread_mutex_destroy(&singleton()->philos[i].meal_mutex);
		pthread_mutex_destroy(&singleton()->fork_mutexes[i]);
		i++;
	}
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
	setup_signals(SIG_RESET);
	if (singleton()->philos)
		free(singleton()->philos);
	singleton()->philos = NULL;
	if (singleton()->fork_mutexes)
		free(singleton()->fork_mutexes);
	singleton()->fork_mutexes = NULL;
	if (singleton()->data)
		free(singleton()->data);
	singleton()->data = NULL;
}
