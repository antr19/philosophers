/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmarsha <fmarsha@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/03 13:12:55 by fmarsha           #+#    #+#             */
/*   Updated: 2022/07/03 15:04:50 by fmarsha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>

long long	timestamp(void)
{
	struct timeval	t;

	gettimeofday(&t, NULL);
	return ((t.tv_sec * 1000) + (t.tv_usec / 1000));
}

void	action_print(t_rules *rules, int id, char *string)
{
	pthread_mutex_lock(&(rules->writing));
	if (!(rules->died))
	{
		printf("%lli ", timestamp() - rules->first_timestamp);
		printf("%i %s\n", id + 1, string);
	}
	pthread_mutex_unlock(&(rules->writing));
	return ;
}

int	write_error(char *s)
{
	printf("Error: %s\n", s);
	return (1);
}

void	ft_sleep(long long time, t_rules *rules)
{
	long long	i;

	i = timestamp();
	while (!(rules->died))
	{
		if (timestamp() - i >= time)
			break ;
		usleep(50);
	}
}

int	ft_atoi(const char *str)
{
	int	num;
	int	sign;

	num = 0;
	sign = -1;
	if (*str == '-')
		str++;
	else
		sign = 1;
	while (*str)
	{
		if (*str >= '0' && *str <= '9')
			num = num * 10 + (*str - '0');
		else
			return (-1);
		str++;
	}
	return (num * sign);
}
