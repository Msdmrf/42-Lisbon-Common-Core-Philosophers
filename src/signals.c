/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migusant <migusant@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 09:18:48 by migusant          #+#    #+#             */
/*   Updated: 2026/03/05 13:26:53 by migusant         ###   ########.fr       */
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
        stop_simulation();
}

static void	handle_sigquit(int sig)
{
	(void)sig;
	if (singleton()->data)
	{
		stop_simulation();
		write(STDOUT_FILENO, "Quit\n", 5);
	}
}

void	setup_signals(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, handle_sigquit);
}

void	reset_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}
