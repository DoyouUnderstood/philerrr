/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alletond <alletond@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 16:06:12 by alletond          #+#    #+#             */
/*   Updated: 2024/02/10 17:10:32 by alletond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_mutexes(t_table *table)
{
	if (safe_mutex_init(&table->write_mutex) != 0 ||
	    safe_mutex_init(&table->table_mutex) != 0)
		return (-1);
	return (0);
}

int	init_forks(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->philo_nbr)
	{
		if (safe_mutex_init(&table->forks[i].fork) != 0)
			return (-1);
		table->forks[i].fork_id = i;
		i++;
	}
	return (0);
}

int	init_philos_and_forks(t_table *table)
{
	table->philos = safe_malloc((table->philo_nbr) * sizeof(t_philo));
    printf("1 : ici le leaks %p\n", table->philos);
	if (!table->philos)
		return (-1);
	table->forks = safe_malloc(table->philo_nbr * sizeof(t_fork));
	if (!table->forks)
	{
		free(table->philos);
		return (-1);
	}
	if (init_mutexes(table) != 0 || init_forks(table) != 0)
	{
		free(table->philos);
		free(table->forks);
		return (-1);
	}
	return (0);
}
