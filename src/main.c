/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omed <omed@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 16:11:14 by omed              #+#    #+#             */
/*   Updated: 2024/09/12 17:17:23 by omed             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minilibx-linux/mlx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdarg.h>

#define ESC_KEY 65307


typedef struct	s_data {
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}				t_data;


typedef struct  s_vars {
    void *mlx;
    void *win;
} t_vars;

typedef struct s_coords {
    int iso_x;
    int iso_y;
    int color;
}   t_coords;

typedef struct s_draw {
    int x0;
    int y0;
    int x1;
    int y1;
    int start_color;
    int end_color;
    int					current_color;
    int start_r;
    int start_g;
    int start_b;
    int end_r;
    int end_g;
    int end_b;
    double current_r;
    double current_g;
    double current_b;
    double color_step_r;
    double color_step_b;
    double color_step_g;
    int dx;
    int dy;
    int sx;
    int sy;
    int err;
    int e2;
    int steps;
} t_draw;

typedef struct s_fdf
{
	t_coords			*coords;
	t_coords			look_at;
	double				depth;
	double				distance;
	double				rot_x;
	double				rot_y;
	double				rot_z;
	int					x_axis;
	int					y_axis;
	int					z_max;
	int					z_min;
	int					z_mod;
	int					index;
	char				*name;
	void				*mlx;
	void				*mlx_win;
	int					is_left_button_pressed;
	int					is_right_button_pressed;
	int					is_sphere;
	int					initial_x;
	int					initial_y;
	int					keep_parallel;
	double				acceleration;
	t_data				img;
}						t_fdf;
void	my_mlx_pixel_put(t_data *data, int x, int y, int color);

void ft_free_array(va_list *args, int c_array)
{
    int i;
    void    **ptr;
    
    i = 0;
    while(i < c_array)
    {
        ptr = va_arg(*args, void**);
        if (ptr != NULL && *ptr != NULL)
        {
            free(*ptr);
            *ptr = NULL;
        }
        i++;
    }
}

void ft_free_matrix(va_list *args, int c_matrix)
{
    int i;
    int j;
    void ***array_ptr;

    i = 0;
    while(i < c_matrix)
    {
        array_ptr = va_arg(*args, void ***);
        if (array_ptr != NULL && *array_ptr)
        {
            j = 0;
            while((*array_ptr)[j] != NULL)
            {
                free((*array_ptr)[j]);
                j++;
            }
            free(*array_ptr);
            *array_ptr = NULL;
        }
    }
}

void ft_free_anything(int c_array, int c_matrix, ...)
{
    va_list args;
    
    va_start(args, c_matrix);
    if (c_array > 0)
        ft_free_array(&args, c_array);
    if (c_matrix > 0)
        ft_free_matrix(&args, c_matrix);
    va_end(args);       
}

void    init_coords_and_colors (t_draw *draw, t_coords start, t_coords end)
{
    draw->x0 = start.iso_x;
    draw->y0 = start.iso_y;
    draw->x1 = end.iso_x;
    draw->y1 = end.iso_y;
    draw->start_color = start.color;
    draw->end_color = end.color;

    draw->start_r = (draw->start_color >> 16) & 0xFF;
    draw->start_g = (draw->start_color >> 8) & 0xFF;
    draw->start_b = draw->start_color & 0xFF;
    
    draw->end_r = (draw->end_color >> 16) & 0xFF;
    draw->end_g = (draw->end_color >> 8) & 0xFF;
    draw->end_b = draw->end_color & 0xFF;
    
    draw->current_r = draw->start_r;
    draw->current_g = draw->start_g;
    draw->current_b = draw->start_b;
}

void    calculate_drawing_params(t_draw *draw) 
{
    draw->dx = abs(draw->x1 - draw->x0);
    draw->dy = -abs(draw->y1 - draw->y0);

    // Set step direction without using the ternary operator
    if (draw->x0 < draw->x1)
        draw->sx = 1;
    else
        draw->sx = -1;

    if (draw->y0 < draw->y1)
        draw->sy = 1;
    else
        draw->sy = -1;

    draw->err = draw->dx + draw->dy;
    draw->e2 = 0;

    // Calculate the maximum steps required
    draw->steps = (abs(draw->x1 - draw->x0) > abs(draw->y1 - draw->y0)) ? abs(draw->x1 - draw->x0) : abs(draw->y1 - draw->y0);

    // Calculate the step size for each color channel
    draw->color_step_r = (double)(draw->end_r - draw->start_r) / draw->steps;
    draw->color_step_g = (double)(draw->end_g - draw->start_g) / draw->steps;
    draw->color_step_b = (double)(draw->end_b - draw->start_b) / draw->steps;
}

void    set_draw(t_draw *draw, t_coords start, t_coords end)
{
    init_coords_and_colors(draw, start, end);
    calculate_drawing_params(draw);
}
 void set_current_color(t_draw *draw) 
 {
    draw->current_color = ((int)draw->current_r << 16)
                        | ((int)draw->current_g << 8)
                        | (int)draw->current_b;
}

void drawline(t_fdf *fdf, t_coords start, t_coords end) {
    t_draw draw;

    set_draw(&draw, start, end);

    while (!(draw.x0 == draw.x1 && draw.y0 == draw.y1)) {
        set_current_color(&draw);
        
        // Plot the current pixel
        my_mlx_pixel_put(&fdf->img, draw.x0, draw.y0, draw.current_color);

        draw.e2 = 2 * draw.err;

        if (draw.e2 >= draw.dy) {
            draw.err += draw.dy;
            draw.x0 += draw.sx;
        }

        if (draw.e2 <= draw.dx) {
            draw.err += draw.dx;
            draw.y0 += draw.sy;
        }

        // Incrementally update the current color for each step
        draw.current_r += draw.color_step_r;
        draw.current_g += draw.color_step_g;
        draw.current_b += draw.color_step_b;

        // Clamp the color values between 0 and 255
        draw.current_r = fmax(0, fmin(255, draw.current_r));
        draw.current_g = fmax(0, fmin(255, draw.current_g));
        draw.current_b = fmax(0, fmin(255, draw.current_b));
    }
}


void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int*)dst = color;
}

/*
int close(t_vars *vars, t_fdf *fdf)
{
    if(fdf != NULL && fdf->mlx != NULL)
    {
    //mlx_destroy_display(fdf->mlx, vars->img.img);
    mlx_destroy_window(vars->mlx, vars->win);
    mlx_destroy_display(vars->mlx);
    ft_free_anything(3,0,&vars->mlx, &fdf->coords, &fdf->name);
    }
	exit(EXIT_SUCCESS);
}*/

int close(t_fdf *fdf)
{
    if (fdf != NULL && fdf->mlx != NULL)
    {
        // Destroy the window and display
        if (fdf->mlx_win != NULL)
            mlx_destroy_window(fdf->mlx, fdf->mlx_win);

        mlx_destroy_display(fdf->mlx);

        // Free dynamically allocated resources
        if (fdf->coords != NULL)
            free(fdf->coords);
        if (fdf->name != NULL)
            free(fdf->name);
        
        // Don't free fdf->mlx, as it is not dynamically allocated
    }
    exit(EXIT_SUCCESS);
}



int	key_hook(int keycode, t_fdf *fdf)
{
    if (keycode == ESC_KEY)
    {
       close(fdf);
    }
    return (0);
}

int mouse_hooks(int mousecode, int x, int y)
{
    if (mousecode == 1)
    {
	    printf("Hello from left_key_hook at %d %d!\n", x, y);
        
    }

    if (mousecode == 3)
    {
	    printf("Hello from right_key_hook at %d %d!\n", x, y);
    }
    return (0);
}

/*
int main()
{
    t_data  img;
    t_vars  vars;
    
    vars.mlx = mlx_init();
    vars.win = mlx_new_window(vars.mlx, 1000, 2080, "Hello Omed!");
    img.img = mlx_new_image(vars.mlx, 640, 360);
    img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length, &img.endian);
    
    int x = 10;
    int y = 10;
    int a = 100;
    int b = 100;
    while( x <= 100 && y <= 100)
    {
        my_mlx_pixel_put(&img, x, y, 0x00FF0000);
        my_mlx_pixel_put(&img, a, b, 0x00FF0000);
        x++;
        y++;
        a--;
        b++;
    }
    
	mlx_put_image_to_window(vars.mlx, vars.win, img.img, 0, 0);
    mlx_hook(vars.win, 2, 1L<<0, key_hook, &vars);
    mlx_hook(vars.win, 4, 1L<<2, mouse_hooks, &vars);
    mlx_hook(vars.win, 17, 1L << 17, close, &vars);
    mlx_loop(vars.mlx);
}
*/

int main() {
    t_fdf fdf;
    t_coords start = {50, 50, 0xFF0000}; 
    t_coords end = {200, 200, 0x0000FF};

    
    fdf.mlx = mlx_init(); // Initialize connection to the display
    if (fdf.mlx == NULL)
    {
        printf("Failed to initialize MiniLibX.\n");
        return (1);
    }
    
    // Create a window
    fdf.mlx_win = mlx_new_window(fdf.mlx, 800, 600, "Line Drawing");
    if (fdf.mlx_win == NULL)
    {
        printf("Failed to create window.\n");
        return (1);
    }

    // Create a new image
    fdf.img.img = mlx_new_image(fdf.mlx, 800, 600);
    if (fdf.img.img == NULL)
    {
        printf("Failed to create image.\n");
        return (2);
    }

    // Get the image data address
    fdf.img.addr = mlx_get_data_addr(fdf.img.img, &fdf.img.bits_per_pixel, &fdf.img.line_length, &fdf.img.endian);

    // Draw the line between start and end coordinates
    drawline(&fdf, start, end);

    // Put the image on the window
    mlx_put_image_to_window(fdf.mlx, fdf.mlx_win, fdf.img.img, 0, 0);

    // Set up hooks for closing the window and user input
    mlx_hook(fdf.mlx_win, 2, 1L << 0, key_hook, &fdf);
    mlx_hook(fdf.mlx_win, 17, 0, close, &fdf);

    // Enter the event loop
    mlx_loop(fdf.mlx);
    
    return (0);
}