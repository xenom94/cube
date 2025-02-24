/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iabboudi <iabboudi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 19:37:39 by iabboudi          #+#    #+#             */
/*   Updated: 2025/02/24 20:49:51 by iabboudi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cube.h"

// Example map; make sure map_rows and map_cols are defined in cube.h.
int map[map_rows][map_cols] = {
    {1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,1,0,1},
    {1,0,0,0,0,0,1,0,0,1},
    {1,0,0,1,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,1,0,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1}
};

double norm_angle(double angle)
{
    angle = fmod(angle, 2 * M_PI);
    if (angle < 0)
        angle += 2 * M_PI;
    return (angle);
}

void update_player(t_player *player)
{
    // Update rotation
    player->rotationAngle += player->turnDirection * player->rotationSpeed;
    player->rotationAngle = norm_angle(player->rotationAngle);

    // Update position for forward/backward movement
    float moveStep = player->walkDirection * player->moveSpeed;
    double newX = player->x + cos(player->rotationAngle) * moveStep;
    double newY = player->y + sin(player->rotationAngle) * moveStep;

    // Update position for strafing movement
    float strafeStep = player->strafeDirection * player->moveSpeed;
    newX += -sin(player->rotationAngle) * strafeStep;
    newY += cos(player->rotationAngle) * strafeStep;
    
    double lookaheadDistance = 5.0;
    if (player->walkDirection != 0 && is_facing_wall(player, lookaheadDistance))
        return;
        
    int mapX = newX / TILE_SIZE;
    int mapY = newY / TILE_SIZE;
    // Collision check: ensure new position is not a wall.
    if (mapX >= 0 && mapX < map_cols && mapY >= 0 && mapY < map_rows && map[mapY][mapX] == 0)
    {
        player->x = newX;
        player->y = newY;
    }
}

void draw_cell(t_player *player, int x, int y, int tile_size, int color)
{
    int i, j;
    i = 0;
    while (i < tile_size)
    {
        j = 0;
        while(j < tile_size)
        {
            my_mlx_pixel_put(player, x + j, y + i, color);
            j++;
        }
        i++;
    }
}

void draw_filled_circle(void *mlx, void *win, t_player *player, int color)
{
    int x = -player->radius;
    while (x <= player->radius)
    {
        int y = -player->radius;
        while (y <= player->radius)
        {
            if (x * x + y * y <= player->radius * player->radius)
                my_mlx_pixel_put(player, player->x + x, player->y + y, color);
            y++;
        }
        x++;
    }
}

void draw_line(t_player *player, int x1, int y1, int x2, int y2, int color)
{
    int deltax = abs(x2 - x1);
    int deltay = abs(y2 - y1);
    int sx, sy;
    int err;

    if (x1 < x2)
        sx = 1;
    else 
        sx = -1;
    if (y1 < y2)
        sy = 1;
    else
        sy = -1;
    err = deltax - deltay;

    while (x1 != x2 || y1 != y2)
    {
        my_mlx_pixel_put(player, x1, y1, color);
        int e2 = err * 2;
        if (e2 > -deltay)
        {
            err -= deltay;
            x1 += sx;
        }
        if (e2 < deltax)
        {
            err += deltax;
            y1 += sy;
        }
    } 
}



// --- NEW FUNCTION: Casting a Single Ray ---
// This function casts a ray at a given angle, stopping if a wall is hit.
// It draws the ray pixel-by-pixel.
void cast_single_ray(t_player *player, double ray_angle, int max_length, int color)
{
    double dx = cos(ray_angle);
    double dy = sin(ray_angle);
    
    // Start from player's position.
    double rayX = player->x;
    double rayY = player->y;
    
    // Step size for the ray. Adjust for accuracy.
    double step = 0.1;
    int distance = 0;
    double lookaheadDistance = 5.0;

    
    while (distance < max_length)
    {
        rayX += dx * step;
        rayY += dy * step;
        distance += step;
        
        // If the ray goes out of bounds, break.
        int mapX = rayX / TILE_SIZE;
        int mapY = rayY / TILE_SIZE;
        if (mapX < 0 || mapX >= map_cols || mapY < 0 || mapY >= map_rows )
            break;
        

        // If a wall is hit, break.
        if (map[mapY][mapX] != 0 )
            break;
        
        // Draw the ray pixel (for visualization).
        my_mlx_pixel_put(player, (int)rayX, (int)rayY, color);
    }
}

// --- NEW FUNCTION: Drawing Multiple Rays ---
void draw_rays(t_player *player, int max_length, int color)
{
    int i;
    // Define field-of-view (FOV) in radians (e.g., 60°).
    double fov = 1.047; // ~60° in radians
    // Number of rays to cast.
    int num_rays = WIN_WIDTH; // For example, one ray per screen column.
    
    // Calculate the starting angle (leftmost ray).
    double start_angle = player->rotationAngle - (fov / 2);
    // Angular step between consecutive rays.
    double angle_step = fov / num_rays;
    i = 0;
    while( i < num_rays)
    {
        double ray_angle = start_angle + i * angle_step;
        ray_angle = norm_angle(ray_angle);
        cast_single_ray(player, ray_angle, max_length, color);
        i++;
    }
}

bool is_facing_wall(t_player *player, double lookaheadDistance)
{
    double lookaheadX = player->x + cos(player->rotationAngle) + lookaheadDistance;
    double lookaheadY = player->y + sin(player->rotationAngle) * lookaheadDistance;
    
    int mapX = lookaheadX / TILE_SIZE;
    int mapY = lookaheadY / TILE_SIZE;
    if (mapX < 0 || mapX >= map_cols || mapY < 0 || mapY >= map_rows)
            return false;
        
        // If a wall is hit, break.
        if (map[mapY][mapX] != 0)
            return true;
    
    return false;
}

void render(t_player *player)
{
    int i, j, pix, color;
    int total_pixels = WIN_WIDTH * WIN_HEIGHT;
    
    // Clear the image buffer.
    pix = 0;
    while (pix < total_pixels)
    {
        ((unsigned int*)player->img_addr)[pix] = 0x000000;
    pix++;
    }
    
    // Draw the map.
    i = 0;
    while ( i < map_rows)
    {
        j = 0;
        while(j < map_cols)
        {
            if (map[i][j] == 1)
                color = 0x000055;
            else 
                color = 0xFFFFFF;
            draw_cell(player, j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, color);
            j++;
        }
        i++;
    }
    
    // Draw the player (as a filled circle).
    draw_filled_circle(player->mlx, player->win, player, 0xFF0000);
    // Draw the player's facing direction line.    
    // Draw multiple rays from the player's position.
    draw_rays(player, 100, 0xFFFF00);  // Rays drawn in yellow with a max length of 300.
    
    // Blit the off-screen image to the window.
    mlx_put_image_to_window(player->mlx, player->win, player->img, 0, 0);
}

void innit_player(t_player *player, void *mlx, void *win)
{
    player->mlx = mlx;
    player->win = win;
    player->x = WIN_WIDTH / 2;
    player->y = WIN_HEIGHT / 2;
    player->radius = 6;
    player->turnDirection = 0;   // -1 for left, +1 for right
    player->walkDirection = 0;   // +1 for forward, -1 for backward
    player->strafeDirection = 0;
    player->rotationAngle = M_PI / 2;  // Starting angle (facing "up")
    player->moveSpeed = 2.0;
    player->rotationSpeed = 2 * (M_PI / 180);  // ~2° per frame in radians
}

int handle_key(int key, t_player *player)
{
    if (key == 65307)  // Escape key
        exit(0);
    if (key == 119)  // W key: move forward
        player->walkDirection = 1;
    if (key == 115)  // S key: move backward
        player->walkDirection = -1;
    if (key == 97)   // A key: strafe left
        player->strafeDirection = -1;
    if (key == 100)  // D key: strafe right
        player->strafeDirection = 1;
    if (key == 65361)   // Left arrow: rotate left
        player->turnDirection = -1;
    if (key == 65363)  // Right arrow: rotate right
        player->turnDirection = 1;
    return (0);
}

int key_release(int key, t_player *player)
{
    if (key == 119 || key == 115)  // W/S released
        player->walkDirection = 0;
    if (key == 65361 || key == 65363)   // Left/Right arrow released
        player->turnDirection = 0;
    if (key == 97 || key == 100)  // A/D released
        player->strafeDirection = 0;
    return (0);
}

int game_loop(t_player *player)
{
    update_player(player);
    render(player);
    return (0);
}

void my_mlx_pixel_put(t_player *player, int x, int y, int color)
{
    char *dst;
    dst = player->img_addr + (y * player->line_length + x * (player->bits_per_pixel / 8));
    *(unsigned int*)dst = color;
}

int main()
{
    t_player player;
    
    player.mlx = mlx_init();
    if (!player.mlx)
    {
        printf("mlx failed to initialize\n");
        return 1;
    }
    
    player.win = mlx_new_window(player.mlx, WIN_WIDTH, WIN_HEIGHT, "raycasting");
    if (!player.win)
    {
        printf("window failed to initialize\n");
        return 1;
    }
    
    innit_player(&player, player.mlx, player.win);
    
    // Initialize the off-screen image.
    player.img = mlx_new_image(player.mlx, WIN_WIDTH, WIN_HEIGHT);
    player.img_addr = mlx_get_data_addr(player.img, &player.bits_per_pixel, &player.line_length, &player.endian);
    
    mlx_hook(player.win, 2, 1L << 0, handle_key, &player);
    mlx_hook(player.win, 3, 1L << 1, key_release, &player);
    mlx_loop_hook(player.mlx, game_loop, &player);
    mlx_loop(player.mlx);
    
    return 0;
}