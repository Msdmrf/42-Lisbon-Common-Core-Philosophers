/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migusant <migusant@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 15:31:25 by migusant          #+#    #+#             */
/*   Updated: 2026/03/13 19:11:47 by migusant         ###   ########.fr       */
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
			return (kill_all_processes());
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

static bool	handle_process_exit(int status)
{
	if (WIFEXITED(status) && WEXITSTATUS(status) == 1)
	{
		stop_simulation(singleton()->data);
		kill_all_processes();
		return (true);
	}
	return (false);
}

static void	wait_remaining_processes(int remaining)
{
	while (remaining > 0)
	{
		waitpid(-1, NULL, 0);
		remaining--;
	}
}

void	wait_processes(void)
{
	pid_t	pid;
	int		status;
	int		processes_remaining;
	bool	process_interrupted;

	processes_remaining = singleton()->processes_created;
	process_interrupted = false;
	while (processes_remaining > 0)
	{
		pid = waitpid(-1, &status, 0);
		if (pid == -1)
			break ;
		processes_remaining--;
		if (handle_process_exit(status))
		{
			process_interrupted = true;
			wait_remaining_processes(processes_remaining);
			return ;
		}
	}
	if (PHILO_DEBUG && processes_remaining == 0 && !process_interrupted
		&& !is_simulation_stopped(singleton()->data))
		printf("\n=== Simulation Completed ===\n");
}
