/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alletond <alletond@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 15:01:51 by alletond          #+#    #+#             */
/*   Updated: 2024/02/10 16:48:37 by alletond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	safe_mutex_init(t_mtx *mutex)
{
	if (pthread_mutex_init(mutex, NULL) != 0)
	{
		printf("Failed to initialize mutex");
		return (-1);
	}
	return (0);
}

void	assign_forks(t_philo *philo, t_fork *forks, int p_pos, int philo_nbr)
{
	if (philo->id % 2 == 0)
	{
		philo->first_fork = &forks[p_pos];
		philo->second_fork = &forks[(p_pos + 1) % philo_nbr];
	}
	else
	{
		philo->first_fork = &forks[(p_pos + 1) % philo_nbr];
		philo->second_fork = &forks[p_pos];
	}
}

int philo_init(t_table *table)
{
	int	i;

	i = 0;
	table->philos = (t_philo *)malloc(table->philo_nbr * sizeof(t_philo));
	if (!table->philos)
		return (-1);

	while(i < table->philo_nbr)
	{
		table->philos[i].id = i + 1;
		table->philos[i].full = false;
		table->philos[i].meals_counter = 0;
		table->philos[i].table = table;
		if (safe_mutex_init(&table->philos[i].philo_mutex) != 0)
		{
			while (--i >= 0)
				pthread_mutex_destroy(&table->philos[i].philo_mutex);
			free(table->philos);
			return (-1);
		}
		assign_forks(&table->philos[i], table->forks, i, table->philo_nbr);
		i++;
	}
	return (0);
}


int	data_init(t_table *table)
{
	table->end_simulation = false;
	table->all_threads_ready = false;
	table->threads_running_nbr = 0;

	if (init_philos_and_forks(table) != 0)
		return (-1);
	if (philo_init(table) != 0) 
	{
		free(table->philos);
		free(table->forks);
		return (-1);
	}
	return (0);
}

