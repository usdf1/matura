#include "TextureManager.h"
#include <SDL_image.h>
#include "Game.h"

TextureManager* TextureManager::s_instance = nullptr;

bool TextureManager::Load(std::string id, std::string fileName)
{
	SDL_Surface* surface = IMG_Load(fileName.c_str());
	if (surface == nullptr) {
		return false;
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(Game::GetInstance()->GetRenderer(), surface);
	if (texture == NULL) {
		return false;
	}

	m_TextureMap[id] = texture;
	return true;
}


//void TextureManager::Draw_cam(std::string id, int x, int y, int width, int height, SDL_RendererFlip flip)
//{
//	SDL_Rect srcRect = { 0,0, width, height };
//	Vector2D cam = Camera::GetInstance().GetPosition();
//	SDL_Rect destRect = { x - cam.X, y - cam.Y, width, height };
//
//	SDL_RenderCopyEx(Game::GetInstance()->GetRenderer(), m_TextureMap[id], NULL, &destRect, 0, NULL, flip);
//}

void TextureManager::Draw(std::string id, int x, int y, float width, float height, SDL_RendererFlip flip)
{

	Uint8 alpha = 170;
	Uint8 alpha2 = 190;
	SDL_SetTextureAlphaMod(m_TextureMap["scit"], alpha);
	SDL_SetTextureAlphaMod(m_TextureMap["end"], alpha2);

	SDL_Rect srcRect = { 0,0, width, height };
	SDL_Rect destRect = { x , y, width, height };

	SDL_RenderCopyEx(Game::GetInstance()->GetRenderer(), m_TextureMap[id], NULL, &destRect, 0, NULL, flip);
}

void TextureManager::DrawFrames(std::string id, int x, int y, float width, float height, int row, int frame, float scale, SDL_RendererFlip flip)
{
	Uint8 alpha = 210;
	SDL_SetTextureAlphaMod(m_TextureMap["gumb"], alpha);
	SDL_SetTextureAlphaMod(m_TextureMap["prijava"], alpha);
	SDL_SetTextureAlphaMod(m_TextureMap["ustvari"], alpha);
	SDL_Rect srcRect = { width * frame, height * row, width, height };
	SDL_Rect destRect = { x, y, width * scale, height * scale };

	SDL_RenderCopyEx(Game::GetInstance()->GetRenderer(), m_TextureMap[id], &srcRect, &destRect, 0, NULL, flip);
}

void TextureManager::DrawAngleFrames(std::string id, int x, int y, int width, int height, int row, int frame, float scale, int angle, SDL_RendererFlip flip)
{
	SDL_Rect srcRect = { width * frame, height * row, width, height };
	SDL_Rect destRect = { x, y, width * scale, height * scale };

	SDL_RenderCopyEx(Game::GetInstance()->GetRenderer(), m_TextureMap[id], &srcRect, &destRect, angle, NULL, flip);

}

void TextureManager::DrawWheel(std::string id, int x, int y, int width, int height, double angle, SDL_RendererFlip flip)
{
	SDL_Rect srcRect = { 0,0, width, height };
	SDL_Rect destRect = { x , y, width, height };
	SDL_RenderCopyEx(Game::GetInstance()->GetRenderer(), m_TextureMap[id], NULL, &destRect, angle, NULL, flip);

}

void TextureManager::renderText(const std::string& text, SDL_Color color, Text& tex, int x, int y) {
	SDL_Surface* textSurface = TTF_RenderText_Solid(tex.mFont, text.c_str(), color);
	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(Game::GetInstance()->GetRenderer(), textSurface);

	SDL_Rect destRect = { x, y, textSurface->w, textSurface->h };
	SDL_RenderCopy(Game::GetInstance()->GetRenderer(), textTexture, NULL, &destRect);

	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(textTexture);
}



void TextureManager::Drop(std::string id)
{
	SDL_DestroyTexture(m_TextureMap[id]);
	m_TextureMap.erase(id);
}

void TextureManager::Clean()
{

}