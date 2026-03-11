/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migusant <migusant@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 15:31:46 by migusant          #+#    #+#             */
/*   Updated: 2026/03/11 11:42:25 by migusant         ###   ########.fr       */
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
		kill(singleton()->pids[i], SIGKILL);
		i++;
	}
}

void	cleanup_and_exit(void)
{
	if (singleton()->data)
	{
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
	}
	setup_signals(SIG_RESET);
	if (singleton()->pids)
		free(singleton()->pids);
	singleton()->pids = NULL;
	if (singleton()->data)
		free(singleton()->data);
	singleton()->data = NULL;
}
