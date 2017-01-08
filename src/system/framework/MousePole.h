/** Copyright (C) 2010-2012 by Jason L. McKesson **/
/** This file is licensed under the MIT License. **/


#ifndef FRAMEWORK_MOUSE_POLE_H
#define FRAMEWORK_MOUSE_POLE_H

#include "window.hpp"
#include <glm/glm.hpp>
#include <glutil/MousePoles.h>

namespace Framework
{
  inline int calc_window_modifiers(Window* window)
	{
		int ret = 0;

    int modifiers = window->getModifierKeys();
    if(modifiers & Window::MOD_SHIFT)
			ret |= glutil::MM_KEY_SHIFT;
    if(modifiers & Window::MOD_CONTROL)
			ret |= glutil::MM_KEY_CTRL;
    if(modifiers & Window::MOD_ALT)
			ret |= glutil::MM_KEY_ALT;

		return ret;
	}

	template<typename Pole>
  inline void ForwardMouseMotion(Pole &forward, int x, int y)
	{
		forward.MouseMove(glm::ivec2(x, y));
	}

	template<typename Pole>
  inline void ForwardMouseButton(Pole &forward, Window* win, int button, int state)
	{
    int modifiers = calc_window_modifiers(win);
    std::pair<double, double> pos = win->getCursorPos();

		glutil::MouseButtons eButton;

		switch(button)
		{
    case Window::MOUSE_LEFT:
			eButton = glutil::MB_LEFT_BTN;
			break;
    case Window::MOUSE_MIDDLE:
			eButton = glutil::MB_MIDDLE_BTN;
			break;
    case Window::MOUSE_RIGHT:
			eButton = glutil::MB_RIGHT_BTN;
			break;
		default:
			return;
		}

    forward.MouseClick(eButton, state == Window::PRESS, modifiers, glm::ivec2((int)pos.first, (int)pos.second));
	}

	template<typename Pole>
  inline void ForwardMouseWheel(Pole &forward, Window* win, double offset)
	{
    std::pair<double, double> pos = win->getCursorPos();
    forward.MouseWheel((int)offset, calc_window_modifiers(win), glm::ivec2((int)pos.first, (int)pos.second));
	}

}

#endif //FRAMEWORK_MOUSE_POLE_H
