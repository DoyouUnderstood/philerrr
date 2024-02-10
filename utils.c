/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alletond <alletond@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 15:12:34 by alletond          #+#    #+#             */
/*   Updated: 2024/02/10 17:08:37 by alletond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>
#include <stdlib.h>

long	gettime(int time_code)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
		printf("failed");
		// error_exit("Gettimeofday failed");
	if (MILLISECOND == time_code)
		return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
	else if (MICROSECOND == time_code)
		return (tv.tv_sec * 1000000 + tv.tv_usec);
	else if (SECONDS == time_code)
		return (tv.tv_sec);
	// else
	// 	error_exit("Invalid time_code for gettime");
	return (0);
}

void	precise_usleep(long usec, t_table *s_table)
{
	long	start;
	long	elapsed;

	start = gettime(MICROSECOND);
	while (gettime(MICROSECOND) - start < usec)
	{
		if (simu_finished(s_table))
			break ;
		elapsed = gettime(MICROSECOND) - start;
		if (usec - elapsed > 10000)
			usleep((usec - elapsed) / 2);
	}
}

void	clean(t_table *s_table)
{
	t_philo	*s_philo;
	int		i;

	i = 0;
	while (i < s_table->philo_nbr)
	{
		s_philo = s_table->philos + i;
		safe_mutex_handle(&s_philo->philo_mutex, DESTROY);
		i++;
	}
	safe_mutex_handle(&s_table->write_mutex, DESTROY);
	safe_mutex_handle(&s_table->table_mutex, DESTROY);
	free(s_table->forks);
	free(s_table->philos);
}

void	error_exit(const char *error)
{
	printf("Error: %s\n", error);
	exit(EXIT_FAILURE);
}

int	check_input(long time_die, long time_eat, long time_sleep)
{
	if (time_die == -1 || time_eat == -1 || time_sleep == -1)
	{
		printf("Erreur dans les inputs !\n");
		return (-1);
	}
	return (0);
}

void	*alone_philo(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	wait_all_threads(philo->table);
	pthread_mutex_lock(&philo->philo_mutex);
	philo->last_meal_time = gettime(MILLISECOND);
	pthread_mutex_unlock(&philo->philo_mutex);
	pthread_mutex_lock(&philo->table->table_mutex);
	philo->table->threads_running_nbr++;
	pthread_mutex_unlock(&philo->table->table_mutex);
	write_status(TAKE_FIRST_FORK, philo);
	while (!simu_finished(philo->table))
		precise_usleep(200, philo->table);
	return (NULL);
}
