
#define b BUTTON_UP
#define isDown(b) input->buttons[b].isDown
#define pressed(b) (input->buttons[b].isDown && input->buttons[b].changed)
#define released(b) (!input->buttons[b].isDown && input->buttons[b].changed)

float playerPos_x = 0.f;
float playerPos_y = 0.f;

internal void
simulateGame(Input* input) {
    clearScreen(0x000000);
    if (isDown(BUTTON_UP)) playerPos_y += 1.2f;
    if (isDown(BUTTON_DOWN)) playerPos_y -= 1.2f;
    if (isDown(BUTTON_LEFT)) playerPos_x -= 1.2f;
    if (isDown(BUTTON_RIGHT)) playerPos_x += 1.2f;
    drawRect(playerPos_x, playerPos_y, 2, 7, 0x00ff22);
}