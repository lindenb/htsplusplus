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

#ifndef HTS_GEOM_HH
#define HTS_GEOM_HH

#include <vector>
#include <cmath>

namespace htspp {

template<typename T>
struct TPoint {
	T x;
	T y;
	TPoint():x(0),y(0) {}
	TPoint(T u,T v):x(u),y(v) {}
	double distance(T X, T Y) const {
	return ::sqrt(std::pow(x-X,2)+std::pow(y-Y,2));
	}
	double distance(const TPoint<T>& o) const {
	return distance(o.x,o.y);
	}
};



template<typename T>
struct TLine {
	T x1;
	T y1;
	T x2;
	T y2;
	TLine():x1(0),y1(0),x2(0),y2(0) {}
	TLine(T x1,T y1, T x2, T y2):x1(x1),y1(y1),x2(x2),y2(y2) {}
	
};


template<typename T>
struct TRect {
	T x;
	T y;
	T width;
	T height;
	TRect():x(0),y(0),width(0),height(0) {}
	TRect(T x,T y, T width, T height):x(x),y(y),width(width),height(height) {}
};

template<typename T>
struct TDimension {
	T width;
	T height;
};



template<typename T>
class TPolygon {
private:
	std::vector<TPoint<T> > points;
	TPoint<T>& last() { return points[points.size()-1];}
public:
	TPolygon() {}
	TPolygon(const TPolygon& cp):points(cp.points) {}
	virtual ~TPolygon() {}
	virtual void clear() { points.clear();}
	virtual bool empty() { return points.empty();}
	virtual TPoint<T> at(int i) const { return points[i];}
	virtual T x(int i) const { return points[i].x;}
	virtual T y(int i) const { return points[i].y;}
	virtual int size() { return (int)points.size();}
	virtual TPolygon<T> add(TPoint<T> p) {points.push_back(p); return *this;}
	virtual TPolygon<T> add(T x,T y) {TPoint<T> p(x,y);return add(p);}
	virtual TPolygon<T> h(T x) {return add(x,last().y);}
	virtual TPolygon<T> v(T y) {return add(last().x,y);}
	virtual TPolygon<T> dh(T x) {return add(x-last().x,last().y);}
	virtual TPolygon<T> dv(T y) {return add(last().x,y-last().y);}
	virtual void close() {add(x(0),y(0));}
};



typedef TPoint<int> Point;
typedef TPoint<double> Point2D;
typedef TLine<int> Line;
typedef TLine<double> Line2D;
typedef TRect<int> Rectangle;
typedef TRect<double> Rectangle2D;
typedef TDimension<int> Dimension;
typedef TDimension<double> Dimension2D;
typedef TPolygon<int> Polygon;
typedef TPolygon<double> Polygon2D;

}

#endif
