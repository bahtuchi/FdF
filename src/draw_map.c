/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omed <omed@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 21:11:12 by omed              #+#    #+#             */
/*   Updated: 2024/10/22 21:11:44 by omed             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

#include "fdf.h"

// Helper function to draw a line between two projected points
void draw_projected_line(t_data *img, t_point2d start, t_point3d point, t_vars *vars, t_point2d offset, int color)
{
    t_point2d projected_end;

    project_iso(point, &projected_end, vars);
    projected_end = point_offset(projected_end, offset);
    draw_line_between_points(img, start, projected_end, color);
}

// Helper function to process points in a row and draw lines
void process_row_lines(t_data *img, t_map *map, t_vars *vars, t_point2d offset, int y)
{
    t_point3d point;
    t_point2d projected_start;
    int x;

    x = 0;
    while (x < map->width)
    {
        point = (t_point3d){x, y, .z = map->z_matrix[y][x]};
        project_iso(point, &projected_start, vars);
        projected_start = point_offset(projected_start, offset);

        if (x < map->width - 1)
        {
            point = (t_point3d){x + 1, y, map->z_matrix[y][x + 1]};
            draw_projected_line(img, projected_start, point, vars, offset, map->color_matrix[y][x]);
        }

        if (y < map->height - 1)
        {
            point = (t_point3d){x, y + 1, map->z_matrix[y + 1][x]};
            draw_projected_line(img, projected_start, point, vars, offset, map->color_matrix[y][x]);
        }

        x++;
    }
}

// Function to draw the entire map
void draw_map(t_data *img, t_map *map, t_vars *vars, t_point2d *offset)
{
    int x;

	x = 0;
    while (x < map->height)
    {
        process_row_lines(img, map, vars, *offset, x);
        x++;
    }
}
