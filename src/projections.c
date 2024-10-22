/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   projections.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omed <omed@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 21:07:34 by omed              #+#    #+#             */
/*   Updated: 2024/10/22 21:13:15 by omed             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void update_projection_bounds(t_point3d point, t_point2d *min, t_point2d *max, t_vars *vars)
{
    t_point2d projected;

    project_iso(point, &projected, vars);
    *min = point_min(*min, projected);
    *max = point_max(*max, projected);
}

// Helper function to process points along a row
void process_row_points(t_map *map, t_vars *vars, int y, t_point2d *min, t_point2d *max)
{
    int x;
    t_point3d point;

    x = 0;
    while (x < map->width)
    {
        point = (t_point3d){x, y, .z = map->z_matrix[y][x]};
        update_projection_bounds(point, min, max, vars);

        if (x < map->width - 1)
        {
            point = (t_point3d){x + 1, y, map->z_matrix[y][x + 1]};
            update_projection_bounds(point, min, max, vars);
        }

        if (y < map->height - 1)
        {
            point = (t_point3d){x, y + 1, map->z_matrix[y + 1][x]};
            update_projection_bounds(point, min, max, vars);
        }
        x++;
    }
}

// Function to calculate projection bounds and size
t_point2d calculate_projection(t_map *map, t_vars *vars, t_point2d *offset_out)
{
    t_point2d max;
    t_point2d min;
    int y;

    max = (t_point2d){0};
    min = (t_point2d){INT_MAX, INT_MAX};
    y = 0;

    while (y < map->height)
    {
        process_row_points(map, vars, y, &min, &max);
        y++;
    }

    if (offset_out != NULL)
        *offset_out = point_neg(min);

    return rect_size(max, min);
}
