/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migusant <migusant@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 15:31:21 by migusant          #+#    #+#             */
/*   Updated: 2026/03/18 13:55:55 by migusant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo_bonus.h"

bool	init_semaphores(t_data *data)
{
	unlink_all_semaphores();
	if (!init_forks_sem(data))
		return (false);
	if (!init_print_sem(data))
		return (false);
	if (!init_stop_sem(data))
		return (false);
	if (!init_death_sem(data))
		return (false);
	return (true);
}

pid_t	*init_pids(int count)
{
	pid_t	*pids;

	pids = malloc(sizeof(pid_t) * count);
	if (!pids)
		return (NULL);
	return (pids);
}

bool	init_philo(t_philo *philo, t_data *data, int id)
{
	philo->id = id;
	philo->meals_eaten = 0;
	atomic_store(&philo->time_to_live, data->start_time + data->time_to_die);
	philo->data = data;
	philo->monitor_created = false;
	atomic_store(&philo->monitor_should_stop, false);
	atomic_store(&philo->died, false);
	if (pthread_create(&philo->monitor_thread, NULL,
			monitor_routine, philo) != 0)
		return (false);
	philo->monitor_created = true;
	return (true);
}
