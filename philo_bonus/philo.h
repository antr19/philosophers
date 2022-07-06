/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmarsha <fmarsha@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/30 23:44:11 by fmarsha           #+#    #+#             */
/*   Updated: 2022/07/03 17:29:16 by fmarsha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdlib.h>
# include <semaphore.h>

struct	s_rules;

typedef struct s_philosopher
{
	int					id;
	int					x_ate;
	int					left_fork_id;
	int					right_fork_id;
	long long			t_last_meal;
	struct s_rules		*rules;
	pthread_t			thread_id;
	pid_t				proc_id;
}						t_philosopher;

typedef struct s_rules
{
	int					num;
	int					time_death;
	int					time_eat;
	int					time_sleep;
	int					num_eat;
	int					died;
	long long			first_timestamp;
	sem_t				*meal_check;
	sem_t				*forks;
	sem_t				*writing;
	t_philosopher		*philosophers;
}						t_rules;

int			init(t_rules *r, char **s);
long long	timestamp(void);
void		action_print(t_rules *rules, int id, char *string);
void		ft_sleep(long long time, t_rules *rules);
int			write_error(char *s);
int			ft_atoi(const char *str);
void		exit_launcher(t_rules *rules);

#endif