internal void
clearScreen(u32 color) {
    u32* pixel = (u32*)renderState.memory;
    for (int y = 0; y < renderState.height; y++) {
        for (int x = 0; x < renderState.width; x++) {
            *pixel++ = color;
        }
    }
}

globalVariable float renderScale = 0.01f;

internal void
drawRectInPixels(int x0, int y0, int x1, int y1, u32 color) {

    x0 = clamp(0, x0, renderState.width);
    x1 = clamp(0, x1, renderState.width);
    y0 = clamp(0, y0, renderState.height);
    y1 = clamp(0, y1, renderState.height);


    for (int y = y0; y < y1; y++) {
        u32* pixel = (u32*)renderState.memory + x0 + y*renderState.width;
        for (int x = x0; x < x1; x++) {
            *pixel++ = color;
        }
    }
}

internal void
drawRect(float x, float y, float halfSize_x, float halfSize_y, u32 color) {
    
    x *= renderState.height * renderScale;
    y *= renderState.height * renderScale;
    halfSize_x *= renderState.height * renderScale;
    halfSize_y *= renderState.height * renderScale;

    x += renderState.width / 2.f;
    y += renderState.height / 2.f;

    // Change to pixels
    int x0 = x - halfSize_x;
    int x1 = x + halfSize_x;
    int y0 = y - halfSize_y;
    int y1 = y + halfSize_y;


    drawRectInPixels(x0, y0, x1, y1, color);
}
const char* letters[][7] = {
	" 00",
	"0  0",
	"0  0",
	"0000",
	"0  0",
	"0  0",
	"0  0",

	"000",
	"0  0",
	"0  0",
	"000",
	"0  0",
	"0  0",
	"000",

	" 000",
	"0",
	"0",
	"0",
	"0",
	"0",
	" 000",

	"000",
	"0  0",
	"0  0",
	"0  0",
	"0  0",
	"0  0",
	"000",

	"0000",
	"0",
	"0",
	"000",
	"0",
	"0",
	"0000",

	"0000",
	"0",
	"0",
	"000",
	"0",
	"0",
	"0",

	" 000",
	"0",
	"0",
	"0 00",
	"0  0",
	"0  0",
	" 000",

	"0  0",
	"0  0",
	"0  0",
	"0000",
	"0  0",
	"0  0",
	"0  0",

	"000",
	" 0",
	" 0",
	" 0",
	" 0",
	" 0",
	"000",

	" 000",
	"   0",
	"   0",
	"   0",
	"0  0",
	"0  0",
	" 000",

	"0  0",
	"0  0",
	"0 0",
	"00",
	"0 0",
	"0  0",
	"0  0",

	"0",
	"0",
	"0",
	"0",
	"0",
	"0",
	"0000",

	"00 00",
	"0 0 0",
	"0 0 0",
	"0   0",
	"0   0",
	"0   0",
	"0   0",

	"00  0",
	"0 0 0",
	"0 0 0",
	"0 0 0",
	"0 0 0",
	"0 0 0",
	"0  00",

	"0000",
	"0  0",
	"0  0",
	"0  0",
	"0  0",
	"0  0",
	"0000",

	" 000",
	"0  0",
	"0  0",
	"000",
	"0",
	"0",
	"0",

	" 000 ",
	"0   0",
	"0   0",
	"0   0",
	"0 0 0",
	"0  0 ",
	" 00 0",

	"000",
	"0  0",
	"0  0",
	"000",
	"0  0",
	"0  0",
	"0  0",

	" 000",
	"0",
	"0 ",
	" 00",
	"   0",
	"   0",
	"000 ",

	"000",
	" 0",
	" 0",
	" 0",
	" 0",
	" 0",
	" 0",

	"0  0",
	"0  0",
	"0  0",
	"0  0",
	"0  0",
	"0  0",
	" 00",

	"0   0",
	"0   0",
	"0   0",
	"0   0",
	"0   0",
	" 0 0",
	"  0",

	"0   0 ",
	"0   0",
	"0   0",
	"0 0 0",
	"0 0 0",
	"0 0 0",
	" 0 0 ",

	"0   0",
	"0   0",
	" 0 0",
	"  0",
	" 0 0",
	"0   0",
	"0   0",

	"0   0",
	"0   0",
	" 0 0",
	"  0",
	"  0",
	"  0",
	"  0",

	"0000",
	"   0",
	"  0",
	" 0",
	"0",
	"0",
	"0000",

	"",
	"",
	"",
	"",
	"",
	"",
	"0",

	"   0",
	"  0",
	"  0",
	" 0",
	" 0",
	"0",
	"0",
};

internal void
internal void
draw_text(const char* text, float x, float y, float size, u32 color) {
	float half_size = size * .5f;
	float original_y = y;

	while (*text) {
		if (*text != 32) {
			const char** letter;
			if (*text == 47) letter = letters[27];
			else if (*text == 46) letter = letters[26];
			else letter = letters[*text - 'A'];
			float original_x = x;

			for (int i = 0; i < 7; i++) {
				const char* row = letter[i];
				while (*row) {
					if (*row == '0') {
						drawRect(x, y, half_size, half_size, color);
					}
					x += size;
					row++;
				}
				y -= size;
				x = original_x;
			}
		}
		text++;
		x += size * 6.f;
		y = original_y;
	}
}

internal void
draw_number(int number, float x, float y, float size, u32 color) {
	float half_size = size * .5f;

	bool drew_number = false;
	while (number || !drew_number) {
		drew_number = true;

		int digit = number % 10;
		number = number / 10;

		switch (digit) {
		case 0: {
			drawRect(x - size, y, half_size, 2.5f * size, color);
			drawRect(x + size, y, half_size, 2.5f * size, color);
			drawRect(x, y + size * 2.f, half_size, half_size, color);
			drawRect(x, y - size * 2.f, half_size, half_size, color);
			x -= size * 4.f;
		} break;

		case 1: {
			drawRect(x + size, y, half_size, 2.5f * size, color);
			x -= size * 2.f;
		} break;

		case 2: {
			drawRect(x, y + size * 2.f, 1.5f * size, half_size, color);
			drawRect(x, y, 1.5f * size, half_size, color);
			drawRect(x, y - size * 2.f, 1.5f * size, half_size, color);
			drawRect(x + size, y + size, half_size, half_size, color);
			drawRect(x - size, y - size, half_size, half_size, color);
			x -= size * 4.f;
		} break;

		case 3: {
			drawRect(x - half_size, y + size * 2.f, size, half_size, color);
			drawRect(x - half_size, y, size, half_size, color);
			drawRect(x - half_size, y - size * 2.f, size, half_size, color);
			drawRect(x + size, y, half_size, 2.5f * size, color);
			x -= size * 4.f;
		} break;

		case 4: {
			drawRect(x + size, y, half_size, 2.5f * size, color);
			drawRect(x - size, y + size, half_size, 1.5f * size, color);
			drawRect(x, y, half_size, half_size, color);
			x -= size * 4.f;
		} break;

		case 5: {
			drawRect(x, y + size * 2.f, 1.5f * size, half_size, color);
			drawRect(x, y, 1.5f * size, half_size, color);
			drawRect(x, y - size * 2.f, 1.5f * size, half_size, color);
			drawRect(x - size, y + size, half_size, half_size, color);
			drawRect(x + size, y - size, half_size, half_size, color);
			x -= size * 4.f;
		} break;

		case 6: {
			drawRect(x + half_size, y + size * 2.f, size, half_size, color);
			drawRect(x + half_size, y, size, half_size, color);
			drawRect(x + half_size, y - size * 2.f, size, half_size, color);
			drawRect(x - size, y, half_size, 2.5f * size, color);
			drawRect(x + size, y - size, half_size, half_size, color);
			x -= size * 4.f;
		} break;

		case 7: {
			drawRect(x + size, y, half_size, 2.5f * size, color);
			drawRect(x - half_size, y + size * 2.f, size, half_size, color);
			x -= size * 4.f;
		} break;

		case 8: {
			drawRect(x - size, y, half_size, 2.5f * size, color);
			drawRect(x + size, y, half_size, 2.5f * size, color);
			drawRect(x, y + size * 2.f, half_size, half_size, color);
			drawRect(x, y - size * 2.f, half_size, half_size, color);
			drawRect(x, y, half_size, half_size, color);
			x -= size * 4.f;
		} break;

		case 9: {
			drawRect(x - half_size, y + size * 2.f, size, half_size, color);
			drawRect(x - half_size, y, size, half_size, color);
			drawRect(x - half_size, y - size * 2.f, size, half_size, color);
			drawRect(x + size, y, half_size, 2.5f * size, color);
			drawRect(x - size, y + size, half_size, half_size, color);
			x -= size * 4.f;
		} break;
		}

	}
}