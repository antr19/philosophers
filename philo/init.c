/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmarsha <fmarsha@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 23:25:42 by fmarsha           #+#    #+#             */
/*   Updated: 2022/07/03 15:04:24 by fmarsha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
		rules->philosophers[i].left_fork_id = i;
		rules->philosophers[i].right_fork_id = (i + 1) % rules->num;
		rules->philosophers[i].t_last_meal = 0;
		rules->philosophers[i].rules = rules;
	}
	return (0);
}

int	init_mutex(t_rules *rules)
{
	int	i;

	i = rules->num;
	rules->forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * i);
	if (rules->forks == 0)
		return (1);
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

int	init(t_rules *r, char **s)
{
	r->num = ft_atoi(s[1]);
	r->time_death = ft_atoi(s[2]);
	r->time_eat = ft_atoi(s[3]);
	r->time_sleep = ft_atoi(s[4]);
	r->died = 0;
	r->all_ate = 0;
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
	if (init_mutex(r) || init_philosophers(r))
		return (1);
	return (0);
}
