/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iabboudi <iabboudi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 19:37:39 by iabboudi          #+#    #+#             */
/*   Updated: 2025/02/07 19:38:26 by iabboudi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cube.h"

int map[map_rows][map_cols] = {
    {1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,1,0,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,0,0,1,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,1,0,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1}
};

void draw_cell(void *mlx, void *win, int x, int y, int tile_size, int color)
{
    int i;
    int j;
    i = 0;
    while (i < tile_size)
    {
        j = 0;
        while (j < tile_size)
        {
         mlx_pixel_put(mlx, win, x + j, y + i, color);
         j++;   
        }
        i++;
    }
}

void player()
{
    t_player 
}

int main()
{
    void *mlx;
    void *win;
    int i;
    int j;
    int color;
        
    mlx = mlx_init();
    i = 0;
    if(!mlx)
    {
        printf("mlx failed to initialize\n");
        return 1;
    }
    
    win = mlx_new_window(mlx, WIN_WIDTH, WIN_HEIGHT, "raycasting");
    if (!win)
    {
        printf("window failed to initialize\n");
        return 1;
    }
    
    while (i < map_rows)
    {
        j = 0;
        while (j < map_cols)
        {
            if (map[i][j] == 1)
                color = 0x000055;
            else 
                color = 0xFFFFFF;
            draw_cell(mlx, win, j * tile_size , i * tile_size, tile_size, color);
            j++;
        }
        i++;
    }

    mlx_loop(mlx);
}

