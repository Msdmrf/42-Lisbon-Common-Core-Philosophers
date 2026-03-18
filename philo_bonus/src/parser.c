/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migusant <migusant@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 15:31:15 by migusant          #+#    #+#             */
/*   Updated: 2026/03/18 10:49:18 by migusant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo_bonus.h"

static void	show_usage(void)
{
	printf("Usage: ./philo_bonus number_of_philosophers time_to_die ");
	printf("time_to_eat time_to_sleep ");
	printf("[number_of_times_each_philosopher_must_eat]\n");
}

static int	ft_atoi(const char *str)
{
	int	result;
	int	sign;

	result = 0;
	sign = 1;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		result = result * 10 + (*str - '0');
		str++;
	}
	return (result * sign);
}

static long	ft_atol(const char *str)
{
	long	result;
	int		sign;

	result = 0;
	sign = 1;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		result = result * 10 + (*str - '0');
		str++;
	}
	return (result * sign);
}

static bool	validate_data(t_data *data)
{
	if (data->philo_count <= 0 || data->philo_count > 200)
	{
		printf("Error: invalid philosopher count\n");
		return (false);
	}
	if (data->time_to_die <= 0 || data->time_to_eat <= 0
		|| data->time_to_sleep <= 0)
	{
		printf("Error: invalid time values\n");
		return (false);
	}
	if (data->must_eat_count == 0)
	{
		printf("Error: must eat at least once\n");
		return (false);
	}
	return (true);
}

t_data	*parse_args(int argc, char **argv)
{
	t_data	*data;

	if (argc < 5 || argc > 6)
		return (show_usage(), NULL);
	data = malloc(sizeof(t_data));
	if (!data)
		return (NULL);
	data->philo_count = ft_atoi(argv[1]);
	data->time_to_die = ft_atol(argv[2]);
	data->time_to_eat = ft_atol(argv[3]);
	data->time_to_sleep = ft_atol(argv[4]);
	if (argc == 6)
		data->must_eat_count = ft_atoi(argv[5]);
	else
		data->must_eat_count = INT_MAX;
	data->start_time = 0;
	data->sim_stop = false;
	data->forks_sem = NULL;
	data->print_sem = NULL;
	data->stop_sem = NULL;
	data->death_sem = NULL;
	if (!validate_data(data))
		return (free(data), NULL);
	return (data);
}
