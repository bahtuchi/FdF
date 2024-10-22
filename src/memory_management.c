/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_management.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omed <omed@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 19:39:59 by omed              #+#    #+#             */
/*   Updated: 2024/10/22 19:59:51 by omed             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

// Function to free split arrays
void ft_free_split(char **str)
{
    int i = 0;
    while (str[i])
    {
        free(str[i]);
        i++;
    }
    free(str);
}

// Function to free map data
void free_map(t_map *map)
{
    int y = 0;
    while (y < map->height)
    {
        free(map->z_matrix[y]);
        free(map->color_matrix[y]);
        y++;
    }
    free(map->z_matrix);
    free(map->color_matrix);
}

void    free_vars(t_vars *vars)
{
    free_map(vars->map);
    if (vars->img.img)
        mlx_destroy_image(vars->mlx, vars->img.img);
    if (vars->win)
        mlx_destroy_window(vars->mlx, vars->win);
    mlx_destroy_display(vars->mlx);
    if (vars->mlx)
        free(vars->mlx);
}


int close_window(t_vars *vars)
{
    free_vars(vars);
    exit(0);  // Exit the program after closing the window
    return (0);
}