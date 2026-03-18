/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migusant <migusant@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 15:31:51 by migusant          #+#    #+#             */
/*   Updated: 2026/03/18 15:27:12 by migusant         ###   ########.fr       */
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
	if (__atomic_exchange_n(&handled, 1, __ATOMIC_SEQ_CST))
		return ;
	if (singleton()->data)
	{
		atomic_store(&singleton()->data->sim_interrupt, true);
		sim_stop();
		kill_all_processes();
	}
}

static void	handle_sigterm(int sig)
{
	(void)sig;
	if (singleton()->data)
		sim_stop();
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
