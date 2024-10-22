/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   point_operations.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omed <omed@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 19:51:09 by omed              #+#    #+#             */
/*   Updated: 2024/10/22 19:54:32 by omed             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void rotate_point(t_point3d *point, float angle)
{
    float x_new;
    float y_new;

    x_new = point->x * cos(angle) - point->y * sin(angle);
    y_new = point->x * sin(angle) + point->y * cos(angle);

    point->x = x_new;
    point->y = y_new;
}

// Isometric projection function
void project_iso(t_point3d point, t_point2d *projected, t_vars *vars)
{
	float x;
    float y;
    float z;

    // Apply zoom and z_scale
    point.x *= vars->zoom;
    point.y *= vars->zoom;
    point.z *= vars->zoom / vars->z_scale;

    // Rotate the point around the Z-axis
    rotate_point(&point, vars->angle);

    // Isometric projection formulas
    x = point.x;
    y = point.y;
    z = point.z;

    projected->x = (x - y) * cos(M_PI / 6) + vars->shift_x;
    projected->y = (x + y) * sin(M_PI / 6) - z + vars->shift_y;
}

t_point2d	point_min(t_point2d a, t_point2d b)
{
	t_point2d min;

	if (a.x < b.x)
		min.x = a.x;
	else
		min.x = b.x;

	if (a.y < b.y)
		min.y = a.y;
	else
		min.y = b.y;

	return (min);
}

t_point2d	point_max(t_point2d a, t_point2d b)
{
	t_point2d max;

	if (a.x > b.x)
		max.x = a.x;
	else
		max.x = b.x;

	if (a.y > b.y)
		max.y = a.y;
	else
		max.y = b.y;

	return (max);
}

t_point2d point_offset(t_point2d point, t_point2d offset)
{
    t_point2d new_point;

    new_point.x = point.x + offset.x;
    new_point.y = point.y + offset.y;
    return (new_point);
}

