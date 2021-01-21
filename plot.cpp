#include "plot.h"
#define _USE_MATH_DEFINES
#include <math.h>

Plot *Plot::_plots = nullptr;
unsigned int Plot::_nplots = 0;
double Plot::_minx = 0.0;
double Plot::_miny = 0.0;
double Plot::_maxx = 0.0;
double Plot::_maxy = 0.0;
bool Plot::_registered = false;

void Plot::_window_paint(HWND hwnd)
{
	//Begin paint
	PAINTSTRUCT paint_struct;
	HDC hdc = BeginPaint(hwnd, &paint_struct);
	RECT window_rect;
	GetClientRect(hwnd, &window_rect);

	for (unsigned int nplot = 0; nplot < Plot::_nplots; nplot++)
	{
		Plot *p = _plots + nplot;
		if (p->n > 1)
		{
			//Move to first point
			double x = p->xfunc(p->xuser, 0);
			double y = p->yfunc(p->yuser, 0);
			MoveToEx(hdc, (int)(window_rect.right * (x - _minx) / (_maxx - _minx)),
				(int)(window_rect.bottom - window_rect.bottom * (y - _miny) / (_maxy - _miny)), nullptr);

			//Choose pen
			HPEN hpen = CreatePen(PS_SOLID, 0, p->color);
			HPEN holdpen = (HPEN)SelectObject(hdc, hpen);
			DeleteObject(holdpen);

			//Draw other points
			for (unsigned int i = 1; i < p->n; i++)
			{
				x = p->xfunc(p->xuser, i);
				y = p->yfunc(p->yuser, i);
				LineTo(hdc, (int)(window_rect.right * (x - _minx) / (_maxx - _minx)),
					(int)(window_rect.bottom - window_rect.bottom * (y - _miny) / (_maxy - _miny)));
			}
		}
	}

	EndPaint(hwnd, &paint_struct);
}

LRESULT CALLBACK Plot::_window_proc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	switch (message)
	{
	case WM_PAINT:
		_window_paint(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		WPARAM j;
		return DefWindowProc(hwnd, message, wparam, lparam);
	}
	return 0;
}

bool Plot::plot(unsigned int nplots, Plot plot1, ...)
{
	if (nplots == 0) return false;

	//Copying parameters to global
	Plot::_nplots = nplots;
	if (Plot::_plots == nullptr) Plot::_plots = (Plot*)malloc(nplots * sizeof(Plot));
	else Plot::_plots = (Plot*)realloc(Plot::_plots, nplots * sizeof(Plot));
	if (Plot::_plots == nullptr) return false;
	memcpy(Plot::_plots, &plot1, nplots * sizeof(Plot));

	//Searching minimum and maximum
	Plot::_minx = INFINITY;
	Plot::_miny = INFINITY;
	Plot::_maxx = -INFINITY;
	Plot::_maxy = -INFINITY;
	for (unsigned int nplot = 0; nplot < nplots; nplot++)
	{
		Plot *p = &plot1 + nplot;
		for (unsigned int i = 0; i < p->n; i++)
		{
			double x = p->xfunc(p->xuser, i);
			if (x < Plot::_minx) Plot::_minx = x;
			if (x > Plot::_maxx) Plot::_maxx = x;

			double y = p->yfunc(p->yuser, i);
			if (y < Plot::_miny) Plot::_miny = y;
			if (y > Plot::_maxy) Plot::_maxy = y;
		}
	}

	//Modifying maximum and minimum
	double interval = Plot::_maxx - Plot::_minx;
	if (interval == 0) interval = 1.0;
	else interval *= 0.1;
	Plot::_minx -= interval;
	Plot::_maxx += interval;

	interval = Plot::_maxy - Plot::_miny;
	if (interval == 0) interval = 1.0;
	else interval *= 0.1;
	Plot::_miny -= interval;
	Plot::_maxy += interval;

	if (!Plot::_registered)
	{
		//Registering class
		WNDCLASS windowclass = { 0 };
		windowclass.lpfnWndProc = _window_proc;
		windowclass.style = CS_HREDRAW | CS_VREDRAW;
		windowclass.hInstance = GetModuleHandle(NULL);
		windowclass.lpszClassName = TEXT("plot_window_class");
		windowclass.hbrBackground = (HBRUSH)COLOR_APPWORKSPACE;
		if (RegisterClass(&windowclass) == 0) return false;
		Plot::_registered = true;
	}

	//Creating window
	RECT screen;
	GetWindowRect(GetDesktopWindow(), &screen);
	HWND hwnd = CreateWindow(TEXT("plot_window_class"), TEXT("plot"), WS_VISIBLE | WS_OVERLAPPEDWINDOW,
		screen.right / 4, screen.bottom / 4, screen.right / 2, screen.bottom / 2,
		NULL, NULL, GetModuleHandle(NULL), NULL);
	if (!hwnd) return false;

	//Displaying window
	MSG msg = { 0 };
	int ok = 0;
	while ((ok = GetMessage(&msg, NULL, 0, 0)) != 0)
	{
		if (ok == -1) return false;
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return false;
}