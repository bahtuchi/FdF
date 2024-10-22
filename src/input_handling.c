/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omed <omed@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 19:52:41 by omed              #+#    #+#             */
/*   Updated: 2024/10/22 19:54:44 by omed             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"
// Key event handler
int handle_key(int keycode, t_vars *vars)
{
    if (keycode == KEY_ESC)
        close_window(vars);
    else if (keycode == KEY_PLUS)
        vars->zoom += vars->zoom_step;
    else if (keycode == KEY_MINUS)
        vars->zoom -= vars->zoom_step;
    else if (keycode == KEY_RIGHT)
        vars->shift_x -= vars->map->width / 10;
    else if (keycode == KEY_LEFT)
        vars->shift_x += vars->map->width / 10;
    else if (keycode == KEY_DOWN)
        vars->shift_y -= vars->map->height / 10;
    else if (keycode == KEY_UP)
        vars->shift_y += vars->map->height / 10;
    else if (keycode == 113) // 'q' key to rotate left by 11,25 degrees
        vars->angle -= M_PI / 16;
    else if (keycode == 101) // 'e' key to rotate right by 11,25 degrees
        vars->angle += M_PI / 16;
    else if (keycode == 97) // 'a' key to decrease z-scale
        vars->z_scale *= 1.1;
    else if (keycode == 100 && vars->z_scale > 0.05) // 'd' key to increase z-scale
        vars->z_scale /= 1.1;
    render_screen(vars);
    return (0);
}

// Mouse event handler
int handle_mouse(int button, int x, int y, t_vars *vars)
{
    // Mark parameters as unused to suppress warnings
    (void)x;
    (void)y;

    if (button == 4) // Scroll up
        vars->zoom += 1;
    else if (button == 5) // Scroll down
        vars->zoom -= 1;

    render_screen(vars);

    return (0);
}

t_point2d point_neg(t_point2d p)
{
	p.x = -p.x;
	p.y = -p.y;
	return (p);
}