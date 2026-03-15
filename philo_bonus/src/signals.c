/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migusant <migusant@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 15:31:51 by migusant          #+#    #+#             */
/*   Updated: 2026/03/15 11:59:50 by migusant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo_bonus.h"

t_singleton	*singleton(void)
{
	static t_singleton	instance;

	return (&instance);
}

static void	handle_interrupt(int sig)
{
	static volatile sig_atomic_t	handled = 0;

	(void)sig;
	if (handled)
		return ;
	handled = 1;
	if (singleton()->data)
	{
		stop_simulation(singleton()->data);
		kill_all_processes();
		if (PHILO_DEBUG)
			printf("\n=== Simulation Interrupted ===\n");
	}
}

static void	handle_sigterm(int sig)
{
	(void)sig;
	if (singleton()->data)
		stop_simulation(singleton()->data);
}

void	setup_signals(int mode)
{
	if (mode == SIG_SETUP)
	{
		signal(SIGINT, handle_interrupt);
		signal(SIGQUIT, handle_interrupt);
		signal(SIGTERM, handle_interrupt);
	}
	else if (mode == SIG_RESET)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		signal(SIGTERM, SIG_DFL);
	}
	else if (mode == SIG_CHILD)
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		signal(SIGTERM, handle_sigterm);
	}
}
