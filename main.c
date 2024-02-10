/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alletond <alletond@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 15:04:01 by alletond          #+#    #+#             */
/*   Updated: 2024/02/10 16:00:34 by alletond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int error_exit2(const char *message) {
    printf("%s\n", message);
    return (-1);
}

int main(int ac, char **av) {
    t_table table;

    if (ac != 5 && ac != 6)
        return (error_exit2("Usage: [program] [arg1] [arg2] [arg3] [arg4] (optional arg5)"));
    if (parse_input(&table, av) == -1)
        return (error_exit2("Error: Invalid input"));
    data_init(&table);
    dinner_start(&table);
    clean(&table);
    return 0;
}
