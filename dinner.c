/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alletond <alletond@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 14:50:01 by alletond          #+#    #+#             */
/*   Updated: 2024/02/09 16:46:51 by alletond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	calculate_think_time(t_philo *s_philo)
{
	long	t_eat;
	long	t_sleep;
	long	t_think;
	long	result;

	t_eat = s_philo->table->time_to_eat;
	t_sleep = s_philo->table->time_to_sleep;
	t_think = (t_eat * 2) - t_sleep;
	if (t_think > 0)
		result = (long)(t_think * 0.42);
	else
		result = 0;
	return (result);
}

void	thinking(t_philo *philo, bool pre_simulation)
{
	long	think_time;

	if (!pre_simulation)
		write_status(THINKING, philo);
	if (philo->table->philo_nbr % 2 != 0)
	{
		think_time = calculate_think_time(philo);
		precise_usleep(think_time, philo->table);
	}
}

static void	eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->first_fork->fork);
	write_status(TAKE_FIRST_FORK, philo);
	pthread_mutex_lock(&philo->second_fork->fork);
	write_status(TAKE_SECOND_FORK, philo);
	pthread_mutex_lock(&philo->philo_mutex);
	philo->last_meal_time = gettime(MILLISECOND);
	philo->meals_counter++;
	pthread_mutex_unlock(&philo->philo_mutex);
	write_status(EATING, philo);
	precise_usleep(philo->table->time_to_eat, philo->table);
	pthread_mutex_lock(&philo->philo_mutex);
	if (philo->table->nbr_limit_meals > 0
		&& philo->meals_counter >= philo->table->nbr_limit_meals)
	{
		philo->full = true;
	}
	pthread_mutex_unlock(&philo->philo_mutex);
	pthread_mutex_unlock(&philo->first_fork->fork);
	pthread_mutex_unlock(&philo->second_fork->fork);
}

static void	*dinner_simulation(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	wait_all_threads(philo->table);
	set_long(&philo->philo_mutex, &philo->last_meal_time, gettime(MILLISECOND));
	increase_long(&philo->table->table_mutex,
		&philo->table->threads_running_nbr);
	de_synchronize_philos(philo);
	while (!simu_finished(philo->table))
	{
		if (get_bool(&philo->philo_mutex, &philo->full))
			break ;
		eat(philo);
		write_status(SLEEPING, philo);
		precise_usleep(philo->table->time_to_sleep, philo->table);
		thinking(philo, false);
	}
	return (NULL);
}

void	dinner_start(t_table *table)
{
	int			i;

	i = -1;
	if (0 == table->nbr_limit_meals)
		return ;
	else if (1 == table->philo_nbr)
		safe_thread_handle(&table->philos[0].thread_id, alone_philo,
			&table->philos[0], CREATE);
	else
		while (++i < table->philo_nbr)
			safe_thread_handle(&table->philos[i].thread_id, dinner_simulation,
				&table->philos[i], CREATE);
	safe_thread_handle(&table->monitor, monitor_dinner, table, CREATE);
	table->start_simulation = gettime(MILLISECOND);
	set_bool(&table->table_mutex, &table->all_threads_ready, true);
	i = -1;
	while (++i < table->philo_nbr)
		safe_thread_handle(&table->philos[i].thread_id, NULL, NULL, JOIN);
	set_bool(&table->table_mutex, &table->end_simulation, true);
	safe_thread_handle(&table->monitor, NULL, NULL, JOIN);
}
