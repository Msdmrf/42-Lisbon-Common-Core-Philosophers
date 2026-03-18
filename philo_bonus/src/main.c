/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migusant <migusant@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 15:31:05 by migusant          #+#    #+#             */
/*   Updated: 2026/03/18 10:43:54 by migusant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo_bonus.h"

int	main(int argc, char **argv)
{
	t_data	*data;
	bool	sems;
	pid_t	*pids;

	data = parse_args(argc, argv);
	if (!data)
		return (1);
	pids = init_pids(data->philo_count);
	sems = init_semaphores(data);
	singleton()->data = data;
	singleton()->pids = pids;
	if (!sems || !pids)
		return (cleanup_resources(CLEANUP_PARENT), 1);
	setup_signals(SIG_SETUP);
	singleton()->data->start_time = get_time_ms();
	start_processes();
	wait_processes();
	cleanup_resources(CLEANUP_PARENT);
	return (0);
}
