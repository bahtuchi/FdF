/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omed <omed@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 19:36:45 by omed              #+#    #+#             */
/*   Updated: 2024/10/22 21:04:42 by omed             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

// Helper function to initialize the map
int initialize_map(int argc, char **argv, t_map *map)
{
    if (argc != 2)
    {
        ft_putstr_fd("Usage: ./fdf <map_file>\n", 1);
        return (1);
    }

    *map = (t_map){0};
    if (load_map(argv[1], map) == -1)
    {
        ft_putstr_fd("Error loading map\n", 1);
        return (1);
    }
    return (0);
}

// Helper function to initialize the MLX library and window
int initialize_mlx(t_vars *vars)
{
    vars->mlx = mlx_init();
    if (!vars->mlx)
    {
        ft_putstr_fd("Error initializing MLX\n", 1);
        return (1);
    }

    vars->win = mlx_new_window(vars->mlx, WINDOW_WIDTH, WINDOW_HEIGHT, "Wireframe Map");
    if (!vars->win)
    {
        ft_putstr_fd("Error creating window\n", 1);
        return (1);
    }
    return (0);
}

// Helper function to initialize transformation parameters
void initialize_vars(t_vars *vars, t_map *map)
{
    t_point2d content_size;
    float zoom_x;
    float zoom_y;

    vars->img = (t_data){0};
    vars->zoom = 1;
    vars->angle = 0;
    vars->z_scale = 1.0;
    vars->shift_x = 0;
    vars->shift_y = 0;
    vars->map = map;

    content_size = calculate_projection(map, vars, 0);
    zoom_x = (float)WINDOW_WIDTH / content_size.x;
    zoom_y = (float)WINDOW_HEIGHT / content_size.y;
    vars->zoom = fmin(zoom_x, zoom_y);
    vars->zoom_step = vars->zoom / 10;
}

// Helper function to set up event hooks
void setup_event_hooks(t_vars *vars)
{
    mlx_key_hook(vars->win, handle_key, vars);
    mlx_mouse_hook(vars->win, handle_mouse, vars);
    mlx_hook(vars->win, 17, 1L << 17, close_window, vars);
}

// Main function
int main(int argc, char **argv)
{
    t_vars vars;
    t_map map;

    if (initialize_map(argc, argv, &map) != 0)
        return (1);

    if (initialize_mlx(&vars) != 0)
    {
        free_map(&map);
        return (1);
    }

    initialize_vars(&vars, &map);
    render_screen(&vars);
    setup_event_hooks(&vars);
    mlx_loop(vars.mlx);

    free_map(&map);
    return (0);
}