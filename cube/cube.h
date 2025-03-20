/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cube.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iabboudi <iabboudi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 19:49:48 by iabboudi          #+#    #+#             */
/*   Updated: 2025/03/20 02:02:06 by iabboudi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUBE_H
# define CUBE_H

# include <string.h>
# include <stdlib.h>
# include <stdio.h>
# include <math.h>
# include  "mlx.h"
# include <stdbool.h>

const int TILE_SIZE = 62;
const int map_rows = 10;
const int map_cols = 10;
const int WIN_WIDTH = map_cols * TILE_SIZE;
const int WIN_HEIGHT= map_rows * TILE_SIZE;

typedef struct s_player
{
    char **map;
    void    *mlx;
    void    *win;
    double     x;
    double     y;
    double  dx;
    double  dy;
    double  dir_angle;       // if needed
    double   radius;
    double     turnDirection;   // -1 for left, +1 for right, 0 for none
    double     walkDirection;   // +1 for forward, -1 for backward, 0 for none
    double strafeDirection;
    double  rotationAngle;   // current facing angle in radians
    double  moveSpeed;
    double  rotationSpeed;   // in radians per frame

    // Fields for double buffering (off-screen image)
    void    *img;
    char    *img_addr;
    int     bits_per_pixel;
    int     line_length;
    int     endian;
} t_player;



void draw_cell(t_player *player, int x, int y, int tile_size, int color);
void my_mlx_pixel_put(t_player *player, int x, int y, int color);
int	has_wall_at(t_player *player, float x, float y);
void update_player(t_player *player);
void draw_rays(t_player *player, int max_length, int color);
bool is_facing_wall(t_player *player, double lookaheadDistance);


#endif