#pragma once
#include <iostream>
#include <string>
#include <vector>

#include "raylib.h"
#include "https.h"


struct element
{
	Image img{};
	Texture2D tex{};

	std::string text;
	int fontType;
	int textSize{};

	Vector2 size{};
	Vector2 position{};
};

class gui {
public:

	int screenW{};
	int screenH{};

	int windowWidth{};
	int windowHeight{};

	std::vector<element> items;
	std::vector<Font> fonts;


	gui(int sH, int sW, bool res, std::string winText,Image icon);
	~gui();

	void createElement(Image img, Vector2 size, Vector2 position = { 0,0 });
	void createElement(std::string text, int fontType, int textSize, Vector2 position = { 0,0 });
	void createElement(std::string text, int fontType, int textSize, int positionX = 0, int positionY = 0);

	void draw();
	void update();

	bool windowExit();

};

