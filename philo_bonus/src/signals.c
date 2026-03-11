/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migusant <migusant@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 15:31:51 by migusant          #+#    #+#             */
/*   Updated: 2026/03/11 11:42:39 by migusant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

t_singleton	*singleton(void)
{
	static t_singleton	instance;

	return (&instance);
}

static void	handle_sigint(int sig)
{
	(void)sig;
	if (singleton()->data)
	{
		stop_simulation(singleton()->data);
		kill_all_processes();
		if (PHILO_DEBUG)
			print_meal_summary("Interrupted");
	}
}

static void	handle_sigquit(int sig)
{
	(void)sig;
	if (singleton()->data)
	{
		stop_simulation(singleton()->data);
		kill_all_processes();
		if (PHILO_DEBUG)
			print_meal_summary("Interrupted");
		write(STDOUT_FILENO, "Quit\n", 5);
	}
}

void	setup_signals(int mode)
{
	if (mode == SIG_SETUP)
	{
		signal(SIGINT, handle_sigint);
		signal(SIGQUIT, handle_sigquit);
	}
	else if (mode == SIG_RESET)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
	}
}
