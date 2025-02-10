/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cube.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iabboudi <iabboudi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 19:49:48 by iabboudi          #+#    #+#             */
/*   Updated: 2025/02/07 19:14:38 by iabboudi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUBE_H
# define CUBE_H

# include <string.h>
# include <stdlib.h>
# include <stdio.h>
# include <math.h>
# include  "mlx.h"

const int tile_size = 64;
const int map_rows = 10;
const int map_cols = 10;
const int WIN_WIDTH = map_cols * tile_size;
const int WIN_HEIGHT= map_rows * tile_size;
const long Math.PI = 3.14;

typedef struct s_player
{
    int x;
    int y;
    int radius;
    int turnDirection = 0;
    int walkDirection = 0 ;
    long rotationAngle = Math.PI / 2 ;
    long moveSpeed = 3.0;
    rotationSpeed = 3 * (Math.PI / 180); //3 degress per frame
}t_player ;


void draw_cell(void *mlx, void *win, int x, int y, int tile_size, int color);


#endif