/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migusant <migusant@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 15:31:46 by migusant          #+#    #+#             */
/*   Updated: 2026/03/12 13:57:44 by migusant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

void	kill_all_processes(void)
{
	int	i;

	if (!singleton()->pids)
		return ;
	i = 0;
	while (i < singleton()->processes_created)
	{
		if (singleton()->pids[i] > 0)
			kill(singleton()->pids[i], SIGTERM);
		i++;
	}
	usleep(10000);
	i = 0;
	while (i < singleton()->processes_created)
	{
		if (singleton()->pids[i] > 0)
			kill(singleton()->pids[i], SIGKILL);
		i++;
	}
}

static void	cleanup_semaphores(void)
{
	if (!singleton()->data)
		return ;
	if (singleton()->data->forks_sem)
	{
		sem_close(singleton()->data->forks_sem);
		sem_unlink(SEM_FORKS);
	}
	if (singleton()->data->print_sem)
	{
		sem_close(singleton()->data->print_sem);
		sem_unlink(SEM_PRINT);
	}
	if (singleton()->data->stop_sem)
	{
		sem_close(singleton()->data->stop_sem);
		sem_unlink(SEM_STOP);
	}
	if (singleton()->data->death_sem)
	{
		sem_close(singleton()->data->death_sem);
		sem_unlink(SEM_DEATH);
	}
}

void	cleanup_resources(int mode)
{
	if (mode == CLEANUP_PARENT)
	{
		cleanup_semaphores();
		setup_signals(SIG_RESET);
	}
	else if (mode == CLEANUP_CHILD)
	{
		if (singleton()->data)
		{
			if (singleton()->data->forks_sem)
				sem_close(singleton()->data->forks_sem);
			if (singleton()->data->print_sem)
				sem_close(singleton()->data->print_sem);
			if (singleton()->data->stop_sem)
				sem_close(singleton()->data->stop_sem);
			if (singleton()->data->death_sem)
				sem_close(singleton()->data->death_sem);
		}
	}
	if (singleton()->pids)
		free(singleton()->pids);
	singleton()->pids = NULL;
	if (singleton()->data)
		free(singleton()->data);
	singleton()->data = NULL;
}
