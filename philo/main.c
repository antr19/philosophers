/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmarsha <fmarsha@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/30 23:45:44 by fmarsha           #+#    #+#             */
/*   Updated: 2022/07/03 15:00:18 by fmarsha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <unistd.h>

void	philo_eats(t_philosopher *philo)
{
	t_rules	*rules;

	rules = philo->rules;
	pthread_mutex_lock(&(rules->forks[philo->left_fork_id]));
	action_print(rules, philo->id, "has taken a fork");
	pthread_mutex_lock(&(rules->forks[philo->right_fork_id]));
	action_print(rules, philo->id, "has taken a fork");
	pthread_mutex_lock(&(rules->meal_check));
	action_print(rules, philo->id, "is eating");
	philo->t_last_meal = timestamp();
	pthread_mutex_unlock(&(rules->meal_check));
	ft_sleep(rules->time_eat, rules);
	(philo->x_ate)++;
	pthread_mutex_unlock(&(rules->forks[philo->left_fork_id]));
	pthread_mutex_unlock(&(rules->forks[philo->right_fork_id]));
}

void	death_checker(t_rules *r, t_philosopher *p)
{
	int	i;

	while (!(r->all_ate))
	{
		i = -1;
		while (++i < r->num)
		{
			pthread_mutex_lock(&(r->meal_check));
			if (timestamp() - p[i].t_last_meal > r->time_death)
			{
				action_print(r, i, "died");
				r->died = 1;
				pthread_mutex_unlock(&(r->meal_check));
				return ;
			}
			pthread_mutex_unlock(&(r->meal_check));
			usleep(100);
		}
		i = 0;
		while (r->num_eat != -1 && i < r->num && p[i].x_ate >= r->num_eat)
			i++;
		if (i == r->num)
			r->all_ate = 1;
	}
}

void	*p_thread(void *void_philosopher)
{
	int				i;
	t_philosopher	*philo;
	t_rules			*rules;

	i = 0;
	philo = (t_philosopher *)void_philosopher;
	rules = philo->rules;
	if (philo->id % 2)
		usleep(5000);
	while (!(rules->died))
	{
		philo_eats(philo);
		if (rules->all_ate)
			break ;
		action_print(rules, philo->id, "is sleeping");
		ft_sleep(rules->time_sleep, rules);
		action_print(rules, philo->id, "is thinking");
		i++;
	}
	return (NULL);
}

int	launcher(t_rules *rules)
{
	int				i;
	t_philosopher	*phi;

	i = 0;
	phi = rules->philosophers;
	rules->first_timestamp = timestamp();
	while (i < rules->num)
	{
		if (pthread_create(&(phi[i].thread_id), NULL, p_thread, &(phi[i])))
			return (1);
		phi[i].t_last_meal = timestamp();
		i++;
	}
	death_checker(rules, rules->philosophers);
	i = -1;
	while (++i < rules->num)
		pthread_join(phi[i].thread_id, NULL);
	i = -1;
	while (++i < rules->num)
		pthread_mutex_destroy(&(rules->forks[i]));
	pthread_mutex_destroy(&(rules->writing));
	return (0);
}

int	main(int agc, char **agv)
{
	t_rules	rules;

	if (agc != 5 && agc != 6)
		return (write_error("Wrong amount of arguments"));
	if (init(&rules, agv))
		return (write_error("Init error"));
	if (launcher(&rules))
		return (write_error("Thread error"));
	free(rules.forks);
	free(rules.philosophers);
	return (0);
}
