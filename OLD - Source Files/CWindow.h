#ifndef CWINDOW_H
#define CWINDOW_H


#include "stdafx.h"

class CWindow
{
public:
	CWindow();
	~CWindow();

	SDL_Window* getWindow();
	SDL_Renderer* getRenderer();

	int getWidth();
	int getHeight();

	void updateSize(); // updates window size. Called when window has changed

private:
	SDL_Window* m_pWindow;
	SDL_Renderer* m_pRenderer;

	int m_windowPosX, m_windowPosY; // top left of window position from top left of screen
	int m_windowWidth, m_windowHeight;

	void init();
};



#endif // !CWINDOW_H