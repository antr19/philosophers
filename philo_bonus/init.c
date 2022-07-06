/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmarsha <fmarsha@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 23:25:42 by fmarsha           #+#    #+#             */
/*   Updated: 2022/07/03 17:28:59 by fmarsha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <signal.h>

int	init_philosophers(t_rules *rules)
{
	int	i;

	i = rules->num;
	rules->philosophers = (t_philosopher *)malloc(sizeof(t_philosopher) * i);
	if (rules->philosophers == 0)
		return (1);
	while (--i >= 0)
	{
		rules->philosophers[i].id = i;
		rules->philosophers[i].x_ate = 0;
		rules->philosophers[i].t_last_meal = 0;
		rules->philosophers[i].rules = rules;
	}
	return (0);
}

int	init_sem(t_rules *rules)
{
	sem_unlink("/philo_forks");
	sem_unlink("/philo_write");
	sem_unlink("/philo_mealcheck");
	rules->forks = sem_open("/philo_forks", O_CREAT, S_IRWXU, rules->num);
	rules->writing = sem_open("/philo_write", O_CREAT, S_IRWXU, 1);
	rules->meal_check = sem_open("/philo_mealcheck", O_CREAT, S_IRWXU, 1);
	if (rules->forks <= 0 || rules->writing <= 0 || rules->meal_check <= 0)
		return (1);
	return (0);
}

int	init(t_rules *r, char **s)
{
	r->num = ft_atoi(s[1]);
	r->time_death = ft_atoi(s[2]);
	r->time_eat = ft_atoi(s[3]);
	r->time_sleep = ft_atoi(s[4]);
	r->died = 0;
	if (r->num < 2 || r->time_death < 0 || r->time_eat < 0 || r->time_sleep < 0)
		return (1);
	if (s[5])
	{
		r->num_eat = ft_atoi(s[5]);
		if (r->num_eat <= 0)
			return (1);
	}
	else
		r->num_eat = -1;
	if (init_sem(r) || init_philosophers(r))
		return (1);
	return (0);
}

void	exit_launcher(t_rules *rules)
{
	int	i;
	int	ret;

	i = 0;
	while (i < rules->num)
	{
		waitpid(-1, &ret, 0);
		if (ret != 0)
		{
			i = -1;
			while (++i < rules->num)
				kill(rules->philosophers[i].proc_id, 15);
			break ;
		}
		i++;
	}
	sem_close(rules->forks);
	sem_close(rules->writing);
	sem_close(rules->meal_check);
	sem_unlink("/philo_forks");
	sem_unlink("/philo_write");
	sem_unlink("/philo_mealcheck");
}
