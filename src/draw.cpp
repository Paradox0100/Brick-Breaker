
// draws on game objects
// also handles game logic


#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "window.h"
#include <iostream>
#include <vector>

bool bricksExist[10][9];
int bricksX[10][9];
int bricksY[10][9];

int destroyed = 0;

bool game = false;
bool won = false;

int x = 0;
int y = 440;
int w = 70;
int h = 10;

int bx = 10;
int by = 150;
int bw = 10;
int bh = 10;

float bSpeed = 5.0;
float bAngle = 330.0;

float bvx = bSpeed * cos(bAngle * M_PI / 180.0);
float bvy = -bSpeed * sin(bAngle * M_PI / 180.0);

TTF_Font* font = nullptr;
void setup() {
    font = TTF_OpenFont("src/Press_Start_2P/PressStart2P-Regular.ttf", 36);
    if (!font) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
        // Handle the error appropriately
    }

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 10; j++) {
            bricksExist[j][i] = true;
        }
    }


    int currentY = 0;
    int currentX = 1;
    for (int j = 0; j < 9; j++) {
        currentY += 5;
        for (int i = 0; i < 10; i++)
        {
            currentX += 5;
            bricksX[i][j] = currentX;
            bricksY[i][j] = currentY;
            currentX += 62;
        }
        currentY += 10;
        currentX = 1;
    }
}

void drawTextCentered(bool win) {
    SDL_Color color = {255, 255, 255, SDL_ALPHA_OPAQUE};
    SDL_Surface* textSurface;
    if (win) {
        textSurface = TTF_RenderText_Blended(font, "YOU WON!", color);
    } else {
        textSurface = TTF_RenderText_Blended(font, "GAME OVER", color);
    }
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    int textWidth, textHeight;
    SDL_QueryTexture(textTexture, NULL, NULL, &textWidth, &textHeight);

    SDL_Rect textRect = {
        (680 - textWidth) / 2,
        (480 - textHeight) / 2,
        textWidth,
        textHeight
    };

    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);
}

void gameOver() {
    SDL_SetRenderDrawColor(renderer,0,0,0,SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    if (won) {
        drawTextCentered(true);
    } else {
        drawTextCentered(false);
    }

    SDL_RenderPresent(renderer);
}

bool checkBrickCollision() {
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 9; j++) {
            if (bricksExist[i][j]) {
                int brickX = bricksX[i][j];
                int brickY = bricksY[i][j];
                if ((brickX <= bx+bw && brickX+62 >= bx && (brickY <= by+bh && brickY+10 >= by))) {
                    bricksExist[i][j] = false;
                    destroyed++;
                    if (destroyed == 89) {
                        won = true;
                    }
                    if (j >= 0 && j <= 2) {
                        bSpeed = 7.0;
                    }
                    if (j >= 3 && j <= 5) {
                        bSpeed = 6.0;
                    }
                    if (j >= 6 && j<= 8) {
                        bSpeed = 5.0;
                    }
                    return true;
                }
            }
        }
    }
    return false;
}

void handleCollision() {
    if (checkBrickCollision()) {
        if (bAngle < 90 && bAngle > 0) {
            bAngle = (270 + (180 - (90 + bAngle)));
        } else if (bAngle < 180 && bAngle > 90) {
            bAngle = (180 + (180 - (90 + (bAngle - 90))));
        } else if (bAngle < 270 && bAngle > 180) {
            bAngle = (270 + (180 - (90 + (bAngle - 180))));
        } else if (bAngle < 360 && bAngle > 270) {
            bAngle = (180 + (180 - (90 + (bAngle - 270))));
        }
    }
    if (bAngle >= 360) {
        bAngle-= 360;
    }
    // bottom
    if (by+bh >= 480) {
        game = true;
    }
    // top
    if (by <= 0) {
        if (bAngle < 90 && bAngle > 0) {
            bAngle = (270+(180-(90+bAngle)));
        }
        if (bAngle < 180 && bAngle > 90) {
            bAngle = (180+(180-(90+(bAngle-90))));
        }
        if (bAngle == 0 || bAngle == 90 || bAngle == 180 || bAngle == 270) {
            bAngle *= -1;
        }
    }
    // right
    if (bx+bw >= 680) {
        if (bAngle < 90 && bAngle > 0) {
            bAngle = (90+(180-(90+bAngle)));
        }
        if (bAngle < 360.0 && bAngle > 270.0) {
            bAngle = 180+(180-((bAngle-270)+90));
        }
        if (bAngle == 0 || bAngle == 90 || bAngle == 180 || bAngle == 270) {
            bAngle *= -1;
        }
    }
    //left
    if (bx <= 0) {
        if (bAngle < 180 && bAngle > 90) {
            bAngle = 90-(180-(90+(180-bAngle)));
        }
        if (bAngle < 270 && bAngle > 180) {
            bAngle = 270+(180-(90+(bAngle-180)));
        }
        if (bAngle == 0 || bAngle == 90 || bAngle == 180 || bAngle == 270) {
            bAngle *= -1;
        }
    }

    // paddle
    if (bx <= x+w && bx+bw >= x && by+bh >= y && !(by > y+h)) {
        if (bAngle < 360.0 && bAngle > 270.0) {
            bAngle = 180.0-(90.0+(bAngle-270));
        }
        if (bAngle < 270 && bAngle > 180) {
            bAngle = (180-(180-((270-bAngle)+90)));
        }
        if (bAngle == 0 || bAngle == 90 || bAngle == 180 || bAngle == 270) {
            bAngle *= -1;
        }
    }

    bvx = bSpeed * cos(bAngle * M_PI / 180.0);
    bvy = -bSpeed * sin(bAngle * M_PI / 180.0);
}

void updatePos() {
    handleCollision();
    bx += bvx;
    by +=  bvy;
}

void square() {
    updatePos();
    SDL_Rect square;
    square.x = bx;
    square.y = by;
    square.w = bw;
    square.h = bh;

    SDL_RenderDrawRect(renderer, &square);
    SDL_RenderFillRect(renderer, &square);
}

void bricks() {
    uint8_t currentColor = 0;
    for (int j = 0; j < 9; j++) {
        for (int i = 0; i < 10; i++)
        {
            if (bricksExist[i][j]) {
                if (currentColor >= 0 && currentColor <= 2) {
                    SDL_SetRenderDrawColor(renderer, 204, 0, 0, SDL_ALPHA_OPAQUE);
                } else if (currentColor >= 3 && currentColor <= 5){
                    SDL_SetRenderDrawColor(renderer, 0, 0, 204, SDL_ALPHA_OPAQUE);
                } else if (currentColor >= 6 && currentColor <= 8) {
                    SDL_SetRenderDrawColor(renderer, 204, 204, 0, SDL_ALPHA_OPAQUE);
                }
                SDL_Rect brick;
                brick.x = bricksX[i][j];
                brick.y = bricksY[i][j];
                brick.h = 10;
                brick.w = 62;
                SDL_RenderDrawRect(renderer, &brick);
                SDL_RenderFillRect(renderer, &brick);
            }
        }
        currentColor++;
    }
}

void draw() {
	SDL_Rect rectangle;
    rectangle.x = x;
    rectangle.y = y;
    rectangle.w = w;
    rectangle.h = h;

	SDL_SetRenderDrawColor(renderer,0,0,0,SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    // Do our drawing
    SDL_SetRenderDrawColor(renderer,255,255,255,SDL_ALPHA_OPAQUE);
    SDL_RenderDrawRect(renderer,&rectangle);
    SDL_RenderFillRect(renderer, &rectangle);
    
    // render game peice
    bricks();
    SDL_SetRenderDrawColor(renderer,255,255,255,SDL_ALPHA_OPAQUE);
    square();

    // Finally show what we've drawn
    SDL_RenderPresent(renderer);
}
