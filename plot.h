#ifndef IR_PLOT
#define IR_PLOT

#include <Windows.h>

class Plot
{
private:
	static Plot *_plots;
	static unsigned int _nplots;
	static double _minx;
	static double _miny;
	static double _maxx;
	static double _maxy;
	static bool _registered;
	static void _window_paint(HWND hwnd);
	static LRESULT CALLBACK _window_proc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);

public:
	///Callback function used by `plot` function
	///@param user Pointer to data given by user
	///@param index Index of point, `0 <= index < n`
	///@return `X` or `Y` coordinate of the point 
	typedef double(GetValueFunction)(void *user, unsigned int index);

	GetValueFunction *xfunc	= nullptr;	///< Function returning `X` coordinate
	GetValueFunction *yfunc	= nullptr;	///< Function returning `Y` coordinate
	void *xuser				= nullptr;	///< Data given to `xfunc`
	void *yuser				= nullptr;	///< Data given to `yfunc`
	unsigned int n			= 0;		///< Number of points in graph
	union
	{
		unsigned int color	= 0;		///< BGR color of line
		struct
		{
			unsigned char b;
			unsigned char g;
			unsigned char r;
			unsigned char a;
		};
	};
	
	///Plots graphs
	///@param nplots	Number of `Plot` structures given to the function
	///@param plot1		First `Plot` to draw
	static bool plot(unsigned int nplots, Plot plot1, ...);
};

#endif	//#ifndef IR_PLOT