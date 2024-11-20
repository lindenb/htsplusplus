#ifndef PS_CANVAS_HH
#define PS_CANVAS_HH
#include <iostream>
#include "Canvas.hh"

namespace htspp {


class PSCanvas : public Canvas {
	private:
		std::ostream& out;
		double toY(double y);
	public:
		PSCanvas(std::ostream& out,int width,int height);
		virtual ~PSCanvas();
			virtual void line(Line2D L);
			virtual void polygon(Polygon2D pt);
	};


}

#endif
