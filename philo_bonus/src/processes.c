/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migusant <migusant@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 15:31:25 by migusant          #+#    #+#             */
/*   Updated: 2026/03/11 12:52:56 by migusant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

void	start_processes(void)
{
	int		i;
	pid_t	pid;

	singleton()->processes_created = 0;
	i = 0;
	while (i < singleton()->data->philo_count)
	{
		pid = fork();
		if (pid == -1)
		{
			stop_simulation(singleton()->data);
			kill_all_processes();
			return ;
		}
		else if (pid == 0)
		{
			philo_process(singleton()->data, i + 1);
			exit(0);
		}
		else
		{
			singleton()->pids[i] = pid;
			singleton()->processes_created++;
		}
		i++;
	}
}

void	wait_processes(void)
{
	int		i;
	int		status;

	i = 0;
	while (i < singleton()->processes_created)
	{
		waitpid(singleton()->pids[i], &status, 0);
		if (WIFEXITED(status) && WEXITSTATUS(status) == 1)
		{
			stop_simulation(singleton()->data);
			kill_all_processes();
			while (++i < singleton()->processes_created)
				waitpid(singleton()->pids[i], NULL, 0);
			return;
		}
		i++;
	}
}
