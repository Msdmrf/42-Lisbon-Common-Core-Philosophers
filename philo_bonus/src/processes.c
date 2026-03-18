/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migusant <migusant@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 15:31:25 by migusant          #+#    #+#             */
/*   Updated: 2026/03/18 15:43:28 by migusant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo_bonus.h"

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
			sim_stop();
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
		sim_stop();
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

static void	print_simulation_result(bool process_died, int processes_remaining)
{
	if (!PHILO_DEBUG)
		return ;
	if (atomic_load(&singleton()->data->sim_interrupt))
		printf("\n=== Simulation Interrupted ===\n");
	else if (process_died)
		printf("\n=== Simulation Failed ===\n");
	else if (processes_remaining == 0 && !is_sim_stopped())
		printf("\n=== Simulation Completed ===\n");
}

void	wait_processes(void)
{
	pid_t	pid;
	int		status;
	int		processes_remaining;
	bool	process_died;

	processes_remaining = singleton()->processes_created;
	process_died = false;
	while (processes_remaining > 0)
	{
		pid = waitpid(-1, &status, 0);
		if (pid == -1)
			break ;
		processes_remaining--;
		if (handle_process_exit(status))
		{
			process_died = true;
			wait_remaining_processes(processes_remaining);
			break ;
		}
	}
	print_simulation_result(process_died, processes_remaining);
}
