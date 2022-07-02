/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmarsha <fmarsha@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 23:25:42 by fmarsha           #+#    #+#             */
/*   Updated: 2022/07/01 23:29:14 by fmarsha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <pthread.h>

void	init_philosophers(t_rules *rules)
{
	int	i;

	i = rules->num;
	rules->philosophers = (t_philosopher *)malloc(sizeof(t_philosopher) * i);
	while (--i >= 0)
	{
		rules->philosophers[i].id = i;
		rules->philosophers[i].x_ate = 0;
		rules->philosophers[i].left_fork_id = i;
		rules->philosophers[i].right_fork_id = (i + 1) % rules->num;
		rules->philosophers[i].t_last_meal = 0;
		rules->philosophers[i].rules = rules;
	}
}

int	init_mutex(t_rules *rules)
{
	int	i;

	i = rules->num;
	rules->forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * i);
	while (--i >= 0)
	{
		if (pthread_mutex_init(&(rules->forks[i]), NULL))
			return (1);
	}
	if (pthread_mutex_init(&(rules->writing), NULL))
		return (1);
	if (pthread_mutex_init(&(rules->meal_check), NULL))
		return (1);
	return (0);
}

void	init(t_rules *r)
{
	r->num = 5;
	r->time_death = 10;
	r->time_eat = 3;
	r->time_sleep = 2;
	r->num_eat = 0;
	r->died = 0;
	r->all_ate = 0;
	r->first_timestamp = 0;
	init_mutex(r);
	init_philosophers(r);
}
