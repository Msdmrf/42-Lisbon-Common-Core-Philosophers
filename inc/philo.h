/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migusant <migusant@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 10:20:50 by migusant          #+#    #+#             */
/*   Updated: 2026/03/05 13:21:31 by migusant         ###   ########.fr       */
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

// ************************************************************************** //
//                                 SINGLETON                                  //
// ************************************************************************** //

// signals.c
t_singleton		*singleton(void);

// ************************************************************************** //
//                                  PARSING                                   //
// ************************************************************************** //

// parser.c
t_data			*parse_args(int argc, char **argv);
bool			validate_data(t_data *data);
void			print_usage(void);
int				ft_atoi(const char *str);
long			ft_atol(const char *str);

// ************************************************************************** //
//                              INITIALIZATION                                //
// ************************************************************************** //

// init.c
pthread_mutex_t	*init_forks(int count);
t_philo			*init_philos(t_data *data, pthread_mutex_t *forks);
void			destroy_mutexes(void);

// ************************************************************************** //
//                            PHILOSOPHER ROUTINE                             //
// ************************************************************************** //

// routine.c
void			*philo_routine(void *arg);
void			take_forks(t_philo *philo);
void			philo_eat(t_philo *philo);
void			release_forks(t_philo *philo);
void			philo_sleep(t_philo *philo);
void			philo_think(t_philo *philo);

// ************************************************************************** //
//                                MONITORING                                  //
// ************************************************************************** //

// monitor.c
void			monitor_simulation(void);
bool			check_deaths(void);
bool			check_all_ate(void);
bool			is_simulation_stopped(void);
void			stop_simulation(void);

// ************************************************************************** //
//                                 UTILITIES                                  //
// ************************************************************************** //

// utils.c
long			get_time_ms(void);
long			get_elapsed_time(long start_time);
void			print_status(t_philo *philo, char *status);
void			precise_sleep(long milliseconds);

// ************************************************************************** //
//                                   MAIN                                     //
// ************************************************************************** //

// main.c
void			start_threads(void);
void			join_threads(void);
void			free_resources(void);
void			cleanup_and_exit(void);

// ************************************************************************** //
//                                  SIGNALS                                   //
// ************************************************************************** //

// signals.c
void			setup_signals(void);
void			reset_signals(void);

#endif