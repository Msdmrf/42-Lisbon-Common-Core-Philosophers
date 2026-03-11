/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migusant <migusant@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 15:31:21 by migusant          #+#    #+#             */
/*   Updated: 2026/03/11 21:11:13 by migusant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

static void	unlink_all_semaphores(void)
{
	sem_unlink(SEM_FORKS);
	sem_unlink(SEM_PRINT);
	sem_unlink(SEM_STOP);
	sem_unlink(SEM_DEATH);
}

static bool	init_forks_sem(t_data *data)
{
	data->forks_sem = sem_open(SEM_FORKS, O_CREAT | O_EXCL, 0644,
			data->philo_count);
	return (data->forks_sem != SEM_FAILED);
}

static bool	init_print_sem(t_data *data)
{
	data->print_sem = sem_open(SEM_PRINT, O_CREAT | O_EXCL, 0644, 1);
	if (data->print_sem == SEM_FAILED)
	{
		sem_close(data->forks_sem);
		sem_unlink(SEM_FORKS);
		return (false);
	}
	return (true);
}

static bool	init_stop_sem(t_data *data)
{
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

static bool	init_death_sem(t_data *data)
{
	data->death_sem = sem_open(SEM_DEATH, O_CREAT | O_EXCL, 0644, 1);
	if (data->death_sem == SEM_FAILED)
	{
		sem_close(data->forks_sem);
		sem_close(data->print_sem);
		sem_close(data->stop_sem);
		sem_unlink(SEM_FORKS);
		sem_unlink(SEM_PRINT);
		sem_unlink(SEM_STOP);
		return (false);
	}
	return (true);
}

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
