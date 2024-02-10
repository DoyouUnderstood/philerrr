/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_status.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alletond <alletond@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 15:15:07 by alletond          #+#    #+#             */
/*   Updated: 2024/02/09 16:47:37 by alletond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <time.h>

void	write_status(t_philo_status status, t_philo *s_philo)
{
	long	elapsed;

	elapsed = gettime(MILLISECOND) - s_philo->table->start_simulation;
	if (get_bool(&s_philo->philo_mutex, &s_philo->full))
		return ;
	safe_mutex_handle(&s_philo->table->write_mutex, LOCK);
	if ((TAKE_FIRST_FORK == status || TAKE_SECOND_FORK == status)
		&& !simu_finished(s_philo->table))
		printf("%ld    %d took a fork\n", elapsed, s_philo->id);
	else if (EATING == status && !simu_finished(s_philo->table))
		printf("%ld    %d is eating\n", elapsed, s_philo->id);
	else if (SLEEPING == status && !simu_finished(s_philo->table))
		printf("%ld    %d is sleeping\n", elapsed, s_philo->id);
	else if (THINKING == status && !simu_finished(s_philo->table))
		printf("%ld    %d is thinking\n", elapsed, s_philo->id);
	else if (DIED == status)
		printf("%-6ld %d died\n", elapsed, s_philo->id);
	safe_mutex_handle(&s_philo->table->write_mutex, UNLOCK);
}
