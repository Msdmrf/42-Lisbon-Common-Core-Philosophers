/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migusant <migusant@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 10:23:49 by migusant          #+#    #+#             */
/*   Updated: 2026/03/11 11:25:06 by migusant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

void	start_threads(void)
{
	int	i;

	i = 0;
	while (i < singleton()->data->philo_count)
	{
		if (pthread_create(&singleton()->philos[i].thread, NULL,
				philo_routine, &singleton()->philos[i]) != 0)
		{
			stop_simulation();
			return ;
		}
		singleton()->data->threads_created++;
		i++;
	}
}

void	join_threads(void)
{
	int	i;

	i = 0;
	while (i < singleton()->data->threads_created)
	{
		pthread_join(singleton()->philos[i].thread, NULL);
		i++;
	}
}
