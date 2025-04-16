#pragma once

#include "SDL.h"
#include <string>

class Animation {
public:

	Animation() {}

	void Update();
	void Draw(float x, float y, int spritWidth, int spriteHeight);
	void SetProps(std::string textureID, int spriteRow, int frameCnt, int animSpeed, float scale, SDL_RendererFlip flip = SDL_FLIP_NONE);

private:
	float m_Scale;

	int m_SpriteRow, m_SpriteFrame;
	int m_AnimSpeed, m_FrameCount;
	std::string m_TextrueID;
	SDL_RendererFlip m_Flip;

};