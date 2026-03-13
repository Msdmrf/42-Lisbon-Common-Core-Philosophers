/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migusant <migusant@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 15:31:58 by migusant          #+#    #+#             */
/*   Updated: 2026/03/13 16:18:31 by migusant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

long	get_elapsed_time(long start_time)
{
	return (get_time_ms() - start_time);
}

void	print_status(t_philo *philo, char *status)
{
	long	timestamp;

	if (is_simulation_stopped(philo->data))
		return ;
	sem_wait(philo->data->print_sem);
	if (sem_trywait(philo->data->death_sem) == 0)
	{
		if (!is_simulation_stopped(philo->data))
		{
			timestamp = get_elapsed_time(philo->data->start_time);
			printf("%ld %d %s\n", timestamp, philo->id, status);
		}
		sem_post(philo->data->death_sem);
	}
	sem_post(philo->data->print_sem);
}

void	precise_sleep(long ms)
{
	long	start;
	long	elapsed;

	start = get_time_ms();
	while (1)
	{
		elapsed = get_time_ms() - start;
		if (elapsed >= ms)
			break ;
		usleep(500);
	}
}
