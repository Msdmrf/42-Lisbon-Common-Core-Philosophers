/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migusant <migusant@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 15:30:55 by migusant          #+#    #+#             */
/*   Updated: 2026/03/18 15:45:01 by migusant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <fcntl.h>
# include <limits.h>
# include <pthread.h>
# include <semaphore.h>
# include <signal.h>
# include <stdatomic.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <sys/wait.h>
# include <unistd.h>

// Enable Debug Mode
# define PHILO_DEBUG	0

// Philosopher Count Thresholds
# define PHILO_COUNT_SMALL		50
# define PHILO_COUNT_MEDIUM		100
# define PHILO_COUNT_LARGE		150

// Stagger Delay Values
# define STAGGER_DELAY_SMALL	50
# define STAGGER_DELAY_MEDIUM	100
# define STAGGER_DELAY_LARGE	150
# define STAGGER_DELAY_XLARGE	200

// Signal Modes
# define SIG_SETUP		1
# define SIG_RESET		2
# define SIG_CHILD		3

// Semaphore Names
# define SEM_FORKS		"/philo_forks"
# define SEM_PRINT		"/philo_print"
# define SEM_STOP		"/philo_stop"
# define SEM_DEATH		"/philo_death"

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
	bool			sim_stop;
	atomic_bool		sim_interrupt;
	sem_t			*forks_sem;
	sem_t			*print_sem;
	sem_t			*stop_sem;
	sem_t			*death_sem;
}	t_data;

// Philosopher Structure
typedef struct s_philo
{
	int				id;
	atomic_long		time_to_live;
	int				meals_eaten;
	bool			monitor_created;
	atomic_bool		monitor_should_stop;
	atomic_bool		died;
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
bool			init_philo(t_philo *philo, t_data *data, int id);

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
void			philo_cycle(t_philo *philo);

// routine.c
void			philo_process(t_data *data, int id);

// forks.c
int				take_forks(t_philo *philo);
void			release_forks(t_philo *philo);

// monitor.c
bool			is_sim_stopped(void);
void			sim_stop(void);
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