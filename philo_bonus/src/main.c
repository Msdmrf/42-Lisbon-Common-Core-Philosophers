/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migusant <migusant@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 15:31:05 by migusant          #+#    #+#             */
/*   Updated: 2026/03/11 12:49:57 by migusant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

int	main(int argc, char **argv)
{
	t_data	*data;
	pid_t	*pids;

	data = parse_args(argc, argv);
	if (!data)
		return (1);
	if (!init_semaphores(data))
		return (free(data), 1);
	pids = init_pids(data->philo_count);
	singleton()->data = data;
	singleton()->pids = pids;
	if (!pids)
		return (cleanup_and_exit(), 1);
	setup_signals(SIG_SETUP);
	singleton()->data->start_time = get_time_ms();
	start_processes();
	wait_processes();
	cleanup_and_exit();
	return (0);
}
