
// #define b BUTTON_UP
#define isDown(b) input->buttons[b].isDown
#define pressed(b) (input->buttons[b].isDown && input->buttons[b].changed)
#define released(b) (!input->buttons[b].isDown && input->buttons[b].changed)

float playerPos_1, playerPos_2, playerDerivPos_1, playerDerivPos_2;
float mapHalfSize_x = 90, mapHalfSize_y = 48;
float playerHalfSize_x = 1, playerHalfSize_y = 10;
float ballPos_x, ballPos_y, ballDerivPos_x = 100, ballDerivPos_y, ballHalfSize = 1;
int counterP1, counterP2;





internal void
simulateGame(Input* input, float dt) {

    clearScreen(0x3377ff);
    //draw_text("P1 Score", -1, 1, 1, 0xff0000);
    drawRect(0, 0, 90, 48, 0x4cadda);
    drawRect(0, 0, .2, renderState.height, 0x000000);

    float playerDDerivPos = 0.f; // units per second

    if (isDown(BUTTON_DOWN)) playerDDerivPos += 350;
    if (isDown(BUTTON_UP)) playerDDerivPos -= 350;

    float playerDDerivPos_2 = 0.f;

    if (isDown(BUTTON_S)) playerDDerivPos_2 += 350;
    if (isDown(BUTTON_W)) playerDDerivPos_2 -= 350;

    playerDDerivPos -= playerDDerivPos * 3.5f;

    playerPos_1 = playerPos_1 + playerDerivPos_1 * dt + playerDDerivPos * dt * dt * .5f;
    playerDerivPos_1 = playerDerivPos_1 + playerDDerivPos * dt;

    if (playerPos_1 + playerHalfSize_y > mapHalfSize_y) {
        // collision
        playerPos_1 = mapHalfSize_y - playerHalfSize_y;
        playerDerivPos_1 = 0;
    }
    
    else if (playerPos_1 - playerHalfSize_y < -mapHalfSize_y) {
        // collision
        playerPos_1 = -mapHalfSize_y + playerHalfSize_y;
        playerDerivPos_1 = 0;
    }

    if (playerPos_2 + playerHalfSize_y > mapHalfSize_y) {
        // collision
        playerPos_2 = mapHalfSize_y - playerHalfSize_y;
        playerDerivPos_2 = 0;
    }

    else if (playerPos_2 - playerHalfSize_y < -mapHalfSize_y) {
        // collision
        playerPos_2 = -mapHalfSize_y + playerHalfSize_y;
        playerDerivPos_2 = 0;
    }


    ballPos_x += ballDerivPos_x * dt;
    ballPos_y += ballDerivPos_y * dt;

    playerDDerivPos_2 -= playerDDerivPos_2 * 3.5f;

    playerPos_2 = playerPos_2 + playerDerivPos_2 * dt + playerDDerivPos_2 * dt * dt * .5f;
    playerDerivPos_2 = playerDerivPos_2 + playerDDerivPos_2 * dt;

    drawRect(ballPos_x, ballPos_y, 1, 1, 0xffffff); // ball

    if (ballPos_x + ballHalfSize > 85 - playerHalfSize_x &&
        ballPos_x - ballHalfSize < 85 + playerHalfSize_x &&
        ballPos_y + ballHalfSize > playerPos_1 - playerHalfSize_y &&
        ballPos_y + ballHalfSize < playerPos_1 + playerHalfSize_y) {

        ballPos_x = 85 - playerHalfSize_x - ballHalfSize;
        ballDerivPos_x *= -1;
        ballDerivPos_y = (ballPos_y - playerPos_1) * 3 + playerDerivPos_1 * .15f;;

    }
    else if (ballPos_x + ballHalfSize > -85 - playerHalfSize_x &&
        ballPos_x - ballHalfSize < -85 + playerHalfSize_x &&
        ballPos_y + ballHalfSize > playerPos_2 - playerHalfSize_y &&
        ballPos_y + ballHalfSize < playerPos_2 + playerHalfSize_y) {

        ballPos_x = -85 + playerHalfSize_x + ballHalfSize;
        ballDerivPos_x *= -1;
        ballDerivPos_y = (ballPos_y - playerPos_2) * 3 + playerDerivPos_2 * .15f;

    }

    if (ballPos_y + ballHalfSize > mapHalfSize_y) {
        ballPos_y = mapHalfSize_y - ballHalfSize;
        ballDerivPos_y *= -1;
    }
    else if (ballPos_y - ballHalfSize < -mapHalfSize_y) {
        ballPos_y = -mapHalfSize_y + ballHalfSize;
        ballDerivPos_y *= -1;
    }
    
    if (ballPos_x + ballHalfSize > mapHalfSize_x) {
        ballDerivPos_x *= -1;
        ballDerivPos_y = 0;
        ballPos_x = 0;
        ballPos_y = 0;
        counterP1 += 1;
    }
    else if (ballPos_x - ballHalfSize < -mapHalfSize_x) {
        ballDerivPos_x *= -1;
        ballDerivPos_y = 0;
        ballPos_x = 0;
        ballPos_y = 0;
        counterP2 += 1;
    }


    drawRect(-85, playerPos_2, playerHalfSize_x, playerHalfSize_y, 0x223bd8);
    drawRect(85, playerPos_1, playerHalfSize_x, playerHalfSize_y, 0x223bd8);
    
}