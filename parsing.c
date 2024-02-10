/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alletond <alletond@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 15:09:37 by alletond          #+#    #+#             */
/*   Updated: 2024/02/10 15:50:39 by alletond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	is_space(char c)
{
	if (c >= 9 && c <= 13)
		return (true);
	if (c == 32)
		return (true);
	return (false);
}

bool	is_digit(char c)
{
	return (c >= '0' && c <= '9');
}

char	*is_positive_digit_str(char *str)
{
	int	len;

	len = 0;
	while (is_space(*str))
		str++;
	if (*str == '+')
		str++;
	else if (*str == '-')
	{
		printf("Feed me only positive values\n");
		return (NULL);
	}
	if (!is_digit(*str))
	{
		printf("The input is not a correct digit\n");
		return (NULL);
	}
	while (is_digit(*str++))
		len++;
	if (len > 10)
	{
		printf("The value is too big, INT_MAX is the limit\n");
		return (NULL);
	}
	return (str - len - 1);
}

long	ft_atol(char *str)
{
	long	num;

	num = 0;
	str = is_positive_digit_str(str);
	if (str == NULL)
		return (-1);
	while (is_digit(*str))
	{
		num = (num * 10) + (*str - '0');
		str++;
		if (num > INT_MAX)
			return (-1);
	}
	return (num);
}

int	parse_input(t_table *table, char **av)
{
	table->philo_nbr = ft_atol(av[1]);
	if (table->philo_nbr > 200 || table->philo_nbr < 1)
	{
		printf("le nombre de philo doit etre entre 1 est 200 !\n");
		return (-1);
	}
	table->time_to_die = ft_atol(av[2]) * 1000;
	table->time_to_eat = ft_atol(av[3]) * 1000;
	table->time_to_sleep = ft_atol(av[4]) * 1000;
	if (av[5])
		table->nbr_limit_meals = ft_atol(av[5]);
	else
		table->nbr_limit_meals = -1;
	if (table->time_to_die < 60000 || table->time_to_sleep < 60000
		|| table->time_to_eat < 60000)
	{
		printf("Use timestamps major than 60ms\n");
		return (-1);
	}
	if (check_input(table->time_to_die, table->time_to_eat,
			table->time_to_sleep) == -1)
		return (-1);
	return (0);
}
