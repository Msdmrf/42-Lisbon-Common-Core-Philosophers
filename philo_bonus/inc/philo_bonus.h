/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migusant <migusant@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 15:30:55 by migusant          #+#    #+#             */
/*   Updated: 2026/03/15 23:04:30 by migusant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <fcntl.h>
# include <pthread.h>
# include <semaphore.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <sys/wait.h>
# include <unistd.h>

// Enable Debug Mode
# define PHILO_DEBUG	0

// Semaphore Names
# define SEM_FORKS		"/philo_forks"
# define SEM_PRINT		"/philo_print"
# define SEM_STOP		"/philo_stop"
# define SEM_DEATH		"/philo_death"

// Signal Modes
# define SIG_SETUP		1
# define SIG_RESET		2
# define SIG_CHILD		3

// Cleanup Modes
# define CLEANUP_PARENT 0
# define CLEANUP_CHILD  1

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
	sem_t			*forks_sem;
	sem_t			*print_sem;
	sem_t			*stop_sem;
	sem_t			*death_sem;
}	t_data;

// Philosopher Structure
typedef struct s_philo
{
	int				id;
	long			last_meal_time;
	int				meals_eaten;
	bool			monitor_created;
	bool			monitor_should_stop;
	bool			died;
	pthread_t		monitor_thread;
	t_data			*data;
}	t_philo;

// Singleton Structure
typedef struct s_singleton
{
	t_data			*data;
	pid_t			*pids;
	int				processes_created;
}	t_singleton;

// parser.c
t_data			*parse_args(int argc, char **argv);

// init.c
bool			init_semaphores(t_data *data);
pid_t			*init_pids(int count);

// init_utils.c
void			unlink_all_semaphores(void);
bool			init_forks_sem(t_data *data);
bool			init_print_sem(t_data *data);
bool			init_stop_sem(t_data *data);
bool			init_death_sem(t_data *data);

// processes.c
void			start_processes(void);
void			wait_processes(void);

// actions.c
void			philo_eat(t_philo *philo);
void			philo_sleep(t_philo *philo);
void			philo_think(t_philo *philo);

// routine.c
void			philo_process(t_data *data, int id);

// forks.c
int				take_forks(t_philo *philo);
void			release_forks(t_philo *philo);

// monitor.c
bool			is_sim_stopped(t_data *data);
void			sim_stop(t_data *data);
void			*monitor_routine(void *arg);

// cleanup.c
void			kill_all_processes(void);
void			cleanup_resources(int mode);

// signals.c
t_singleton		*singleton(void);
void			setup_signals(int mode);

// utils.c
long			get_time_ms(void);
long			get_elapsed_time(long start_time);
void			print_status(t_philo *philo, char *status);
void			precise_sleep(long ms);

#endif