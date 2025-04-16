#pragma once
#include "Point.h"
#include "Vector2D.h"
#include <SDL.h>

class Camera {
public:
	static Camera& GetInstance() { static Camera instance; return instance; }

	void Update(int scale);
	inline SDL_Rect GetView() { return m_View; }
	inline Vector2D GetPosition() { return m_Position; }
	inline void SetTarget(Point* target) { m_Target = *target; }

private:
	Camera() {}
	SDL_Rect m_View;
	Point m_Target;
	Vector2D m_Position;
};
