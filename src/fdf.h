/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omed <omed@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 20:08:10 by omed              #+#    #+#             */
/*   Updated: 2024/10/22 20:17:09 by omed             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H 

# include "../minilibx-linux/mlx.h"
# include "../libft/libft.h"
# include <fcntl.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <math.h>

# define M_PI 3.14159265358979323846
# define WINDOW_WIDTH 1800
# define WINDOW_HEIGHT 1000

// Keycodes for user input
# define KEY_ESC 65307
# define KEY_PLUS 43
# define KEY_MINUS 45
# define KEY_LEFT 65361
# define KEY_RIGHT 65363
# define KEY_UP 65362
# define KEY_DOWN 65364

// Structure to hold the map data
typedef struct s_map {
    int width;
    int height;
    int **z_matrix;
    int **color_matrix;
} t_map;

// Structure to hold image data
typedef struct s_data {
    void    *img;
    char    *addr;
    int     bits_per_pixel;
    int     line_length;
    int     endian;
} t_data;

// Structure to hold MLX variables and transformation parameters
typedef struct s_vars {
    void    *mlx;
    void    *win;
    t_data  img;
    float   zoom;
    float   zoom_step;
    float   angle;
    float   z_scale;
    int     shift_x;
    int     shift_y;
    t_map   *map; // Correctly typed as t_map *
} t_vars;

// 3D point structure
typedef struct s_point3d {
    float x;
    float y;
    float z;
} t_point3d;

// 2D point structure
typedef struct s_point2d {
    int x;
    int y;
} t_point2d;

// Function declarations
void        ft_free_split(char **str);
void        my_mlx_pixel_put(t_data *data, int x, int y, int color);
int         get_map_dimensions(const char *filename, t_map *map);
int         get_color(char *str);
int         load_map(const char *filename, t_map *map);
void        free_map(t_map *map);
void        rotate_point(t_point3d *point, float angle);
void        project_iso(t_point3d point, t_point2d *projected, t_vars *vars);
void        draw_line_between_points(t_data *data, t_point2d p1, t_point2d p2, int color);
void        render_screen(t_vars *vars);
void        free_vars(t_vars *vars);
int         handle_key(int keycode, t_vars *vars);
int         handle_mouse(int button, int x, int y, t_vars *vars);
int         close_window(t_vars *vars);
void        draw_map(t_data *img, t_map *map, t_vars *vars, t_point2d *offset);
t_point2d   rect_size(t_point2d max, t_point2d min);
t_point2d   calculate_projection(t_map *map, t_vars *vars, t_point2d *offset_out);
t_point2d   rect_center_offset(t_point2d a, t_point2d b);
t_point2d   point_min(t_point2d a, t_point2d b);
t_point2d   point_max(t_point2d a, t_point2d b);
t_point2d   point_offset(t_point2d point, t_point2d offset);
t_point2d   point_neg(t_point2d p);
int         main(int argc, char **argv);

#endif