#include "Animation.h"
#include "TextureManager.h"
#include <iostream>

void Animation::Update()
{
	m_SpriteFrame = (SDL_GetTicks() / m_AnimSpeed) % m_FrameCount;
}

void Animation::Draw(float x, float y, int spritWidth, int spriteHeight)
{
	TextureManager::GetInstance()->DrawFrames(m_TextrueID, x, y, spritWidth, spriteHeight, m_SpriteRow, m_SpriteFrame, m_Scale, m_Flip);
}

void Animation::SetProps(std::string textureID, int spriteRow, int frameCnt, int animSpeed, float scale, SDL_RendererFlip flip)
{
	m_TextrueID = textureID;
	m_SpriteRow = spriteRow;
	m_FrameCount = frameCnt;
	m_AnimSpeed = animSpeed;
	m_Flip = flip;
	m_Scale = scale;
}
