/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omed <omed@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 20:28:37 by omed              #+#    #+#             */
/*   Updated: 2024/10/22 21:02:00 by omed             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

// Function to load map data
int load_map(const char *filename, t_map *map)
{
    int     fd;
    char    *line;
    char    **nums;
    int     y;
    int     x;

    if (get_map_dimensions(filename, map) == -1)
        return (-1);

    map->z_matrix = malloc(sizeof(int *) * map->height);
    if (!map->z_matrix)
        return (-1);

    map->color_matrix = malloc(sizeof(int *) * map->height);
    if (!map->color_matrix)
        return (-1);

    fd = open(filename, O_RDONLY);
    if (fd < 0)
        return (-1);
    y = 0;
    while ((line = get_next_line(fd)) != NULL)
    {
        nums = ft_split(line, ' ');
        if (!nums)
        {
            free(line);
            close(fd);
            return (-1);
        }
        map->z_matrix[y] = malloc(sizeof(int) * map->width);
        map->color_matrix[y] = malloc(sizeof(int) * map->width);
        if (!map->z_matrix[y] || !map->color_matrix[y])
        {
            ft_free_split(nums);
            free(line);
            close(fd);
            return (-1);
        }
        x = 0;
        while (x < map->width)
        {
            map->z_matrix[y][x] = ft_atoi(nums[x]);
            map->color_matrix[y][x] = get_color(nums[x]);
            x++;
        }
        ft_free_split(nums);
        free(line);
        y++;
    }
    close(fd);
    return (0);
}
