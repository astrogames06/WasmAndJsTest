#include <raylib.h>
#include <raymath.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <cstdlib>

#define RAYGUI_IMPLEMENTATION
#include <raygui.h>

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

int num = 0;
extern "C"
{
	EMSCRIPTEN_KEEPALIVE
	int add(int x, int y)
	{
		return x + y;
	}

	EMSCRIPTEN_KEEPALIVE
	int GetNum()
	{
		return num;
	}

	EMSCRIPTEN_KEEPALIVE
	void SetNum(int x)
	{
		num = x;
	}
}

EM_JS(void, SaveNum, (int  x), {
  localStorage.setItem("num", x);
});

const int WIDTH = 850;
const int HEIGHT = 450;

void UpdateDrawFrame();

int main(void)
{
	InitWindow(WIDTH, HEIGHT, "raylib [core] example - basic window");
	#if defined(PLATFORM_WEB)
    	emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
	#else
		SetTargetFPS(60);   // Set our game to run at 60 frames-per-second
		//--------------------------------------------------------------------------------------

		// Main game loop
		while (!WindowShouldClose())    // Detect window close button or ESC key
		{
			UpdateDrawFrame();
		}
	#endif

	CloseWindow();

	return 0;
}

void UpdateDrawFrame()
{
	BeginDrawing();
	
	ClearBackground(RAYWHITE);

	std::string text = "NUM : " + std::to_string(GetNum());
	if (GuiButton({WIDTH/2-100/2, HEIGHT/2, 100, 30}, text.c_str()))
	{
		EM_ASM({
			alert(JSON.parse(localStorage.getItem("num")));
		});
	}

	if (GuiButton({WIDTH/2-100/2, HEIGHT/2+30, 50, 30}, "+"))
	{
		SetNum(num + 1);
		SaveNum(num);
	}

	if (GuiButton({WIDTH/2, HEIGHT/2+30, 50, 30}, "-"))
	{
		SetNum(num - 1);
		SaveNum(num);
	}

	EndDrawing();
}