#include "Color.hh"

using namespace htspp;


static unsigned char to_uchar(int v) {
return (unsigned char)v;
}

Color::Color(int r,int g,int b):r(to_uchar(r)), g(to_uchar(g)), b(to_uchar(b)) {
}

Color::Color(int gray):r(to_uchar(gray)), g(to_uchar(gray)), b(to_uchar(gray)) {
}

Color::~Color() {
}

Color(const Color& cp):r(cp.r), g(cp.g), b(cp.b) {
}


Color::Color():r(to_uchar(0)), g(to_uchar(0)), b(to_uchar(0)) {
}

		int Color::red() const { return (int)r;}
		int Color::green() const{ return (int)g;}
		int Color::blue() const{ return (int)b;}


const Color Color::aliceblue = Color(240,248,255);
const Color Color::antiquewhite = Color(250,235,215);
const Color Color::aqua = Color(0,255,255);
const Color Color::aquamarine = Color(127,255,212);
const Color Color::azure = Color(240,255,255);
const Color Color::beige = Color(245,245,220);
const Color Color::bisque = Color(255,228,196);
const Color Color::black = Color(0,0,0);
const Color Color::blanchedalmond = Color(255,235,205);
const Color Color::blue = Color(0,0,255);
const Color Color::blueviolet = Color(138,43,226);
const Color Color::brown = Color(165,42,42);
const Color Color::burlywood = Color(222,184,135);
const Color Color::cadetblue = Color(95,158,160);
const Color Color::chartreuse = Color(127,255,0);
const Color Color::chocolate = Color(210,105,30);
const Color Color::coral = Color(255,127,80);
const Color Color::cornflowerblue = Color(100,149,237);
const Color Color::cornsilk = Color(255,248,220);
const Color Color::crimson = Color(220,20,60);
const Color Color::cyan = Color(0,255,255);
const Color Color::darkblue = Color(0,0,139);
const Color Color::darkcyan = Color(0,139,139);
const Color Color::darkgoldenrod = Color(184,134,11);
const Color Color::darkgray = Color(169,169,169);
const Color Color::darkgreen = Color(0,100,0);
const Color Color::darkgrey = Color(169,169,169);
const Color Color::darkkhaki = Color(189,183,107);
const Color Color::darkmagenta = Color(139,0,139);
const Color Color::darkolivegreen = Color(85,107,47);
const Color Color::darkorange = Color(255,140,0);
const Color Color::darkorchid = Color(153,50,204);
const Color Color::darkred = Color(139,0,0);
const Color Color::darksalmon = Color(233,150,122);
const Color Color::darkseagreen = Color(143,188,143);
const Color Color::darkslateblue = Color(72,61,139);
const Color Color::darkslategray = Color(47,79,79);
const Color Color::darkslategrey = Color(47,79,79);
const Color Color::darkturquoise = Color(0,206,209);
const Color Color::darkviolet = Color(148,0,211);
const Color Color::deeppink = Color(255,20,147);
const Color Color::deepskyblue = Color(0,191,255);
const Color Color::dimgray = Color(105,105,105);
const Color Color::dimgrey = Color(105,105,105);
const Color Color::dodgerblue = Color(30,144,255);
const Color Color::firebrick = Color(178,34,34);
const Color Color::floralwhite = Color(255,250,240);
const Color Color::forestgreen = Color(34,139,34);
const Color Color::fuchsia = Color(255,0,255);
const Color Color::gainsboro = Color(220,220,220);
const Color Color::ghostwhite = Color(248,248,255);
const Color Color::gold = Color(255,215,0);
const Color Color::goldenrod = Color(218,165,32);
const Color Color::gray = Color(128,128,128);
const Color Color::green = Color(0,128,0);
const Color Color::greenyellow = Color(173,255,47);
const Color Color::grey = Color(128,128,128);
const Color Color::honeydew = Color(240,255,240);
const Color Color::hotpink = Color(255,105,180);
const Color Color::indianred = Color(205,92,92);
const Color Color::indigo = Color(75,0,130);
const Color Color::ivory = Color(255,255,240);
const Color Color::khaki = Color(240,230,140);
const Color Color::lavender = Color(230,230,250);
const Color Color::lavenderblush = Color(255,240,245);
const Color Color::lawngreen = Color(124,252,0);
const Color Color::lemonchiffon = Color(255,250,205);
const Color Color::lightblue = Color(173,216,230);
const Color Color::lightcoral = Color(240,128,128);
const Color Color::lightcyan = Color(224,255,255);
const Color Color::lightgoldenrodyellow = Color(250,250,210);
const Color Color::lightgray = Color(211,211,211);
const Color Color::lightgreen = Color(144,238,144);
const Color Color::lightgrey = Color(211,211,211);
const Color Color::lightpink = Color(255,182,193);
const Color Color::lightsalmon = Color(255,160,122);
const Color Color::lightseagreen = Color(32,178,170);
const Color Color::lightskyblue = Color(135,206,250);
const Color Color::lightslategray = Color(119,136,153);
const Color Color::lightslategrey = Color(119,136,153);
const Color Color::lightsteelblue = Color(176,196,222);
const Color Color::lightyellow = Color(255,255,224);
const Color Color::lime = Color(0,255,0);
const Color Color::limegreen = Color(50,205,50);
const Color Color::linen = Color(250,240,230);
const Color Color::magenta = Color(255,0,255);
const Color Color::maroon = Color(128,0,0);
const Color Color::mediumaquamarine = Color(102,205,170);
const Color Color::mediumblue = Color(0,0,205);
const Color Color::mediumorchid = Color(186,85,211);
const Color Color::mediumpurple = Color(147,112,219);
const Color Color::mediumseagreen = Color(60,179,113);
const Color Color::mediumslateblue = Color(123,104,238);
const Color Color::mediumspringgreen = Color(0,250,154);
const Color Color::mediumturquoise = Color(72,209,204);
const Color Color::mediumvioletred = Color(199,21,133);
const Color Color::midnightblue = Color(25,25,112);
const Color Color::mintcream = Color(245,255,250);
const Color Color::mistyrose = Color(255,228,225);
const Color Color::moccasin = Color(255,228,181);
const Color Color::navajowhite = Color(255,222,173);
const Color Color::navy = Color(0,0,128);
const Color Color::oldlace = Color(253,245,230);
const Color Color::olive = Color(128,128,0);
const Color Color::olivedrab = Color(107,142,35);
const Color Color::orange = Color(255,165,0);
const Color Color::orangered = Color(255,69,0);
const Color Color::orchid = Color(218,112,214);
const Color Color::palegoldenrod = Color(238,232,170);
const Color Color::palegreen = Color(152,251,152);
const Color Color::paleturquoise = Color(175,238,238);
const Color Color::palevioletred = Color(219,112,147);
const Color Color::papayawhip = Color(255,239,213);
const Color Color::peachpuff = Color(255,218,185);
const Color Color::peru = Color(205,133,63);
const Color Color::pink = Color(255,192,203);
const Color Color::plum = Color(221,160,221);
const Color Color::powderblue = Color(176,224,230);
const Color Color::purple = Color(128,0,128);
const Color Color::red = Color(255,0,0);
const Color Color::rosybrown = Color(188,143,143);
const Color Color::royalblue = Color(65,105,225);
const Color Color::saddlebrown = Color(139,69,19);
const Color Color::salmon = Color(250,128,114);
const Color Color::sandybrown = Color(244,164,96);
const Color Color::seagreen = Color(46,139,87);
const Color Color::seashell = Color(255,245,238);
const Color Color::sienna = Color(160,82,45);
const Color Color::silver = Color(192,192,192);
const Color Color::skyblue = Color(135,206,235);
const Color Color::slateblue = Color(106,90,205);
const Color Color::slategray = Color(112,128,144);
const Color Color::slategrey = Color(112,128,144);
const Color Color::snow = Color(255,250,250);
const Color Color::springgreen = Color(0,255,127);
const Color Color::steelblue = Color(70,130,180);
const Color Color::tan = Color(210,180,140);
const Color Color::teal = Color(0,128,128);
const Color Color::thistle = Color(216,191,216);
const Color Color::tomato = Color(255,99,71);
const Color Color::turquoise = Color(64,224,208);
const Color Color::violet = Color(238,130,238);
const Color Color::wheat = Color(245,222,179);
const Color Color::white = Color(255,255,255);
const Color Color::whitesmoke = Color(245,245,245);
const Color Color::yellow = Color(255,255,0);
const Color Color::yellowgreen = Color(154,205,50);
