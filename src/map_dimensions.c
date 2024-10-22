/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_dimensions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omed <omed@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 19:41:31 by omed              #+#    #+#             */
/*   Updated: 2024/10/22 21:12:02 by omed             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

// Helper function to open a file and handle errors
static int open_file(const char *filename)
{
    int fd;
    fd = open(filename, O_RDONLY);
    if (fd < 0)
        perror("Error opening file");
    return (fd);
}

// Helper function to count the number of elements in a line
static int count_line_width(char *line)
{
    char **nums = ft_split(line, ' ');
    int width = 0;

    if (!nums)
        return (-1);
    while (nums[width])
        width++;
    ft_free_split(nums);
    return width;
}

// Helper function to process a line and validate its width
static int process_line(char *line, t_map *map)
{
    int width = count_line_width(line);

    if (width < 0)
    {
        free(line);
        return (-1);
    }
    if (map->width == -1)
        map->width = width;
    else if (map->width != width)
    {
        free(line);
        perror("Error: Inconsistent map width\n");
        return (-1);
    }
    free(line);
    map->height++;
    return (0);
}

// Function to get map dimensions
int get_map_dimensions(const char *filename, t_map *map)
{
    int fd;
    char *line;

    fd = open_file(filename);
    if (fd < 0)
        return (-1);
    
    map->height = 0;
    map->width = -1;
    while ((line = get_next_line(fd)) != NULL)
    {
        if (process_line(line, map) < 0)
        {
            close(fd);
            return (-1);
        }
    }
    close(fd);
    return (0);
}

// input is a string of a number (for the z_matrix), with an optional comma + hex color
int get_color(char *str)
{
    int color;
    int i;

    i = 0;
    // col = white
    color = 0xFFFFFF;
    while (str[i])
    {
        if (str[i] == ',' && str[i+1] == '0' && str[i+2] == 'x')
        {
            i += 3;
            color = ft_atoi_base(&str[i], "0123456789ABCDEF");
            break;
        }
        i++;
    }
    return (color);
}


