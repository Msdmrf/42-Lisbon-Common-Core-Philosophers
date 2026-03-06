/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migusant <migusant@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 10:20:50 by migusant          #+#    #+#             */
/*   Updated: 2026/03/06 20:58:23 by migusant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>
# include <signal.h>

// Signal Modes
# define SIG_SETUP 1
# define SIG_RESET 2

// Data Structure
typedef struct s_data
{
	int				philo_count;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	int				must_eat_count;
	long			start_time;
	bool			simulation_stop;
	pthread_mutex_t	stop_mutex;
	pthread_mutex_t	print_mutex;
}	t_data;

// Philosopher Structure
typedef struct s_philo
{
	int				id;
	pthread_t		thread;
	long			last_meal_time;
	int				meals_eaten;
	pthread_mutex_t	meal_mutex;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	t_data			*data;
}	t_philo;

// Singleton Structure
typedef struct s_singleton
{
	t_data			*data;
	t_philo			*philos;
	pthread_mutex_t	*forks;
}	t_singleton;

// signals.c
t_singleton		*singleton(void);
void			setup_signals(int mode);

// parser.c
t_data			*parse_args(int argc, char **argv);

// init.c
pthread_mutex_t	*init_forks(int count);
t_philo			*init_philos(t_data *data, pthread_mutex_t *forks);
void			destroy_mutexes(void);

// routine.c
void			*philo_routine(void *arg);

// monitor.c
bool			is_simulation_stopped(void);
void			stop_simulation(void);
void			monitor_simulation(void);

// utils.c
long			get_time_ms(void);
void			print_status(t_philo *philo, char *status);
void			precise_sleep(long milliseconds);

#endif