/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   drawing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omed <omed@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 19:44:04 by omed              #+#    #+#             */
/*   Updated: 2024/10/22 21:03:44 by omed             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

// Function to put a pixel to the image
void my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
    char    *dst;

    // Check if the pixel is within the image boundaries
    if (x >= 0 && x < WINDOW_WIDTH && y >= 0 && y < WINDOW_HEIGHT)
    {
        dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
        *(unsigned int*)dst = color;
    }
}
// Line drawing function (Bresenham's Algorithm)
void draw_line_between_points(t_data *data, t_point2d p1, t_point2d p2, int color)
{
    int dx = abs(p2.x - p1.x);
    int dy = -abs(p2.y - p1.y);
    int sx = (p1.x < p2.x) ? 1 : -1;
    int sy = (p1.y < p2.y) ? 1 : -1;
    int error = dx + dy;
    int error2;

    while (1)
    {
        my_mlx_pixel_put(data, p1.x, p1.y, color);
        if (p1.x == p2.x && p1.y == p2.y)
            break;
        error2 = 2 * error;
        if (error2 >= dy)
        {
            error += dy;
            p1.x += sx;
        }
        if (error2 <= dx)
        {
            error += dx;
            p1.y += sy;
        }
    }
}

// Function to draw the wireframe map
void    render_screen(t_vars *vars)
{
    t_point2d center_offset;
    t_point2d content_size;
    t_point2d offset;

     // Redraw the map with updated parameters
    mlx_clear_window(vars->mlx, vars->win);
    if (vars->img.img)
        mlx_destroy_image(vars->mlx, vars->img.img);
    content_size = calculate_projection(vars->map, vars, &offset);
    center_offset = rect_center_offset((t_point2d){WINDOW_WIDTH, WINDOW_HEIGHT}, content_size);
    offset = point_offset(point_offset(center_offset, offset), (t_point2d){vars->shift_x, vars->shift_y});
    vars->img.img = mlx_new_image(vars->mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
    vars->img.addr = mlx_get_data_addr(vars->img.img, &vars->img.bits_per_pixel, &vars->img.line_length, &vars->img.endian);
    draw_map(&vars->img, vars->map, vars, &offset); // Use vars->map
    mlx_put_image_to_window(vars->mlx, vars->win, vars->img.img, 0, 0);
}

t_point2d rect_size(t_point2d max, t_point2d min)
{
    t_point2d size;

    size.x = abs(max.x - min.x);
    size.y = abs(max.y - min.y);
    return (size);
}

t_point2d rect_center_offset(t_point2d a, t_point2d b)
{
    t_point2d offset;

    offset.x = a.x / 2 - b.x / 2;
    offset.y = a.y / 2 - b.y / 2;
    return (offset);
}
