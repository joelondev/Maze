#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define MAP_WIDTH 8
#define MAP_HEIGHT 8
#define FOV_ANGLE 60
#define TILE_SIZE 64

// Example map data
int map[MAP_WIDTH][MAP_HEIGHT] = {
  {1, 1, 1, 1, 1, 1, 1, 1},
  {1, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 1, 0, 1, 1, 0, 1},
  {1, 0, 1, 0, 0, 1, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 1},
  {1, 1, 0, 1, 1, 1, 1, 1},
  {1, 0, 0, 0, 0, 0, 0, 1},
  {1, 1, 1, 1, 1, 1, 1, 1}
};

void drawWallSlice(SDL_Renderer *renderer, int x, double distanceToWall) {
  int wallHeight = (int)(SCREEN_HEIGHT / distanceToWall);

  // Calculate wall color based on distance
  Uint8 wallColor = (Uint8)(255 - distanceToWall * 10);

  SDL_SetRenderDrawColor(renderer, wallColor, wallColor, wallColor, 255);
  SDL_Rect wallRect = {x, (SCREEN_HEIGHT - wallHeight) / 2, 1, wallHeight};
  SDL_RenderFillRect(renderer, &wallRect);
}

int main(int argc, char *argv[]) {
  SDL_Window *window = NULL;
  SDL_Renderer *renderer = NULL;
    
  SDL_Init(SDL_INIT_VIDEO);

  window = SDL_CreateWindow("Raycasting Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  double playerX = 2.5;
  double playerY = 2.5;
  double playerAngle = 90; // Initial player angle in degrees

  while (1) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
	goto cleanup;
      }
      // You can handle other events here, like changing the player angle
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    for (int x = 0; x < SCREEN_WIDTH; x++) {
      double rayAngle = (playerAngle - FOV_ANGLE / 2) + ((double)x / SCREEN_WIDTH) * FOV_ANGLE;
      double rayX = cos(rayAngle * M_PI / 180.0);
      double rayY = sin(rayAngle * M_PI / 180.0);

      double distanceToWall = 0;
      int hitWall = 0;
      int wallX = (int)playerX;
      int wallY = (int)playerY;

      while (!hitWall && distanceToWall < 20) {
	wallX = (int)(playerX + rayX * distanceToWall);
	wallY = (int)(playerY + rayY * distanceToWall);

	if (wallX < 0 || wallX >= MAP_WIDTH || wallY < 0 || wallY >= MAP_HEIGHT) {
	  hitWall = 1;
	  distanceToWall = 20;
	} else if (map[wallX][wallY] == 1) {
	  hitWall = 1;
	} else {
	  distanceToWall += 0.1;
	}
      }

      drawWallSlice(renderer, x, distanceToWall);
    }

    SDL_RenderPresent(renderer);
    SDL_Delay(16); // Cap the frame rate to approximately 60 FPS
  }

 cleanup:
  if (renderer) {
    SDL_DestroyRenderer(renderer);
  }
  if (window) {
    SDL_DestroyWindow(window);
  }
  SDL_Quit();

  return 0;
}
