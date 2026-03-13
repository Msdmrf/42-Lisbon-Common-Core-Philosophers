/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migusant <migusant@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 09:18:48 by migusant          #+#    #+#             */
/*   Updated: 2026/03/13 17:13:50 by migusant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

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
		stop_simulation();
		if (PHILO_DEBUG)
			print_meal_summary("Interrupted");
	}
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
}
