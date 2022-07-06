/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmarsha <fmarsha@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/30 23:45:44 by fmarsha           #+#    #+#             */
/*   Updated: 2022/07/03 17:29:43 by fmarsha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <unistd.h>
#include <stdio.h>

void	philo_eats(t_philosopher *philo)
{
	t_rules	*rules;

	rules = philo->rules;
	sem_wait(rules->forks);
	action_print(rules, philo->id, "has taken a fork");
	sem_wait(rules->forks);
	action_print(rules, philo->id, "has taken a fork");
	sem_wait(rules->meal_check);
	action_print(rules, philo->id, "is eating");
	philo->t_last_meal = timestamp();
	sem_post(rules->meal_check);
	ft_sleep(rules->time_eat, rules);
	(philo->x_ate)++;
	sem_post(rules->forks);
	sem_post(rules->forks);
}

void	*p_thread(void *void_philosopher)
{
	t_philosopher	*p;
	t_rules			*r;

	p = (t_philosopher *)void_philosopher;
	r = p->rules;
	while (1)
	{
		sem_wait(r->meal_check);
		if (timestamp() - p->t_last_meal > r->time_death)
		{
			action_print(r, p->id, "died");
			r->died = 1;
			sem_post(r->meal_check);
			exit(1);
		}
		sem_post(r->meal_check);
		usleep(1000);
		if (p->x_ate >= r->num_eat && r->num_eat != -1)
			break ;
	}
	return (NULL);
}

void	p_process(void *void_phil)
{
	t_philosopher	*philo;
	t_rules			*rules;

	philo = (t_philosopher *)void_phil;
	rules = philo->rules;
	philo->t_last_meal = timestamp();
	pthread_create(&(philo->thread_id), NULL, p_thread, void_phil);
	if (philo->id % 2)
		usleep(15000);
	while (!(rules->died))
	{
		philo_eats(philo);
		if (philo->x_ate >= rules->num_eat && rules->num_eat != -1)
			break ;
		action_print(rules, philo->id, "is sleeping");
		ft_sleep(rules->time_sleep, rules);
		action_print(rules, philo->id, "is thinking");
	}
	pthread_join(philo->thread_id, NULL);
	if (rules->died)
		exit(1);
	exit(0);
}

int	launcher(t_rules *rules)
{
	int				i;
	t_philosopher	*phi;

	i = -1;
	phi = rules->philosophers;
	rules->first_timestamp = timestamp();
	while (++i < rules->num)
	{
		phi[i].proc_id = fork();
		if (phi[i].proc_id < 0)
			return (1);
		else if (phi[i].proc_id == 0)
			p_process(&(phi[i]));
		usleep(100);
	}
	exit_launcher(rules);
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
	free(rules.philosophers);
	return (0);
}
