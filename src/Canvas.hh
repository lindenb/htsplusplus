/*
The MIT License (MIT)

Copyright (c) 2024 Pierre Lindenbaum

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

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
