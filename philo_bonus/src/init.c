/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migusant <migusant@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 15:31:21 by migusant          #+#    #+#             */
/*   Updated: 2026/03/15 11:59:17 by migusant         ###   ########.fr       */
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
