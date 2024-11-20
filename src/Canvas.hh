#ifndef HTSPP_CANVAS_HH
#define HTSPP_CANVAS_HH
#include <stack>
#include "Geom.hh"
#include "Color.hh"
namespace htspp {

class Canvas {
private:
	int _width;
	int _height;
protected:
	class State {
		Color* stroke;
		Color* fill;
		};
	stack<State*> states;
	Canvas(int w, int h);
public:
	virtual ~Canvas();
	virtual int width() const;
	virtual int height() const;
	virtual void push();
	virtual void pop();
	virtual void noStroke();
	virtual void noFill();
	virtual void stroke(Color c);
	virtual void fill(Color c);
	virtual void line(double x1,double y1,double x2,double y2);
	virtual void line(Line2D L)=0;
	virtual void polygon(Polygon2D pt)=0;
	virtual void polyline(Polygon2D pt)=0;
  virtual void hershey( const char* s, 	double x, double y, double width, double height);
  virtual void hershey( const char* s, 	Rectangle2D rect);
};


}

#endif
