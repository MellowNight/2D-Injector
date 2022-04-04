#include "global.h"

namespace Global
{
	HWND	main_window = NULL;
	int		window_height = 0;
	int		window_width = 0;

	bool	d3dhook_inited = false;

	RECT	window_rect;
}