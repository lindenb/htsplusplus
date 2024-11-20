#include "PSCanvas.hh"

using namespace htspp;
using namespace std;

static double inch(double pixel) {
		return pixel;
		}


PSCanvas::PSCanvas(std::ostream& out,int width,int height):Canvas(width,height),out(out) {

		this.w.println("%PS");
		this.w.println("%%Creator: jvarkit");
		this.w.println("%%BoundingBox: 0 0 "+(int)inch(width)+" "+(int)inch(height));
		
		this.w.println("/rd { rlineto } def ");
		for(int i=1;i<=10;i++) {
			this.w.println("/slw"+i+" { "+i+" setlinewidth } def");
		}
		
		this.w.println("/rect { 4 dict begin /h exch def /w exch def /y exch def /x exch def "+
			"newpath "+
			"x y moveto " +
			"w 0 rd " +
			"0 h neg rd " +
			"w neg 0 rd " +
			"0 h rd " +
			"closepath " +
			"end } def"
			);
		this.w.println("/square { 3 dict begin /w exch def /y exch def /x exch def "+
			"x y w w rect " +
			"end } def"
			);
		this.w.println("/ll { 4 dict begin /y2 exch def /x2 exch def /y1 exch def /x1 exch def "+
			"newpath "+
			"x1 y1 moveto " +
			"x2 y2 lineto " +
			"closepath " +
			"end } def"
			);
		this.w.println("/circle { 3 dict begin /r exch def /y exch def /x exch def "+
				"newpath "+
				"x y r 0 360 arc " +
				"closepath " +
				"end } def"
				);
		this.w.println("/hl { 1 dict begin  /dx exch def "+
				"dx 0 rlineto " +
				"end } def"
				);
		this.w.println("/vl { 1 dict begin  /dy exch def "+
				"0 dy neg rd " +
				"end } def"
				);
		out << "/irgb { 3 dict begin /b exch def /g exch def /r exch def "
				"r 255.0 div " 
				"g 255.0 div " 
				"b 255.0 div " 
				" setrgbcolor " 
				"end } def" << endl;
				
		out << "/igray { 1 dict begin /g exch def  "
				" g g g irgb " 
				"end } def"
				<< endl;


}

PSCanvas::~PSCanvas() {
out.flush();
}


double PSCanvas::toY(double y) {
return height()-y;
}

void PSCanvas::line(Line2D L) {
State* state= states.top();
if(state->stroke==NULL) return;

out << " stroke\n";
}

void azd{
		boolean gsave_set=false;
		Color* c= state->stroke;
		if(c!=NULL ) {
			w.print(" gsave ");
			pathIterator(shape);
			w.print(" ");
			w.print(setrgbcolor(c));
			w.print(" fill");
			gsave_set = true;
			}
		
		final double linewidth=getStrokeWidth();
		if(linewidth>0) {
			c= getStrokeColor();
			if(c!=null) {
				if(!gsave_set) {
					w.print(" gsave ");
					pathIterator(shape);
					gsave_set = true;
					}
				w.print(" ");
				w.print(setLineWidth(linewidth));
				w.print(" ");
				w.print(setrgbcolor(c));
				w.print(" stroke");
				}
			}
		if(gsave_set) {
			w.print(" grestore");
			}
	}



void PSCanvas::polygon(Polygon2D pt) {
State* state= states.top();
if(state->stroke==NULL && state->fill==NULL) return;

if(state->fill!=NULL) {
	out << " fill\n";
	}
if(state->stroke!=NULL) {
	out << " stroke\n";
	}
}
