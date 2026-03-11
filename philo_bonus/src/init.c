/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migusant <migusant@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 15:31:21 by migusant          #+#    #+#             */
/*   Updated: 2026/03/11 11:41:15 by migusant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

bool	init_semaphores(t_data *data)
{
	sem_unlink(SEM_FORKS);
	sem_unlink(SEM_PRINT);
	sem_unlink(SEM_STOP);
	data->forks_sem = sem_open(SEM_FORKS, O_CREAT | O_EXCL, 0644,
			data->philo_count);
	if (data->forks_sem == SEM_FAILED)
		return (false);
	data->print_sem = sem_open(SEM_PRINT, O_CREAT | O_EXCL, 0644, 1);
	if (data->print_sem == SEM_FAILED)
	{
		sem_close(data->forks_sem);
		sem_unlink(SEM_FORKS);
		return (false);
	}
	data->stop_sem = sem_open(SEM_STOP, O_CREAT | O_EXCL, 0644, 1);
	if (data->stop_sem == SEM_FAILED)
	{
		sem_close(data->forks_sem);
		sem_close(data->print_sem);
		sem_unlink(SEM_FORKS);
		sem_unlink(SEM_PRINT);
		return (false);
	}
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
