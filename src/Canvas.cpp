#include "Canvas.hh"

using namespace std;
using namespace htspp;



static const int MOVETO=0;
static const int LINETO=1;

struct Operator
	{
	double x;
	double y;
	int operator;
	};

	 
static const char* charToHersheyString(int c)
		{
		switch(c)
			{
			case 'a' : case 'A' : return "  9MWRMNV RRMVV RPSTS"; break;
			case 'b' : case 'B' : return " 16MWOMOV ROMSMUNUPSQ ROQSQURUUSVOV"; break;
			case 'c' : case 'C' : return " 11MXVNTMRMPNOPOSPURVTVVU"; break;
			case 'd' : case 'D' : return " 12MWOMOV ROMRMTNUPUSTURVOV"; break;
			case 'e' : case 'E' : return " 12MWOMOV ROMUM ROQSQ ROVUV"; break;
			case 'f' : case 'F' : return "  9MVOMOV ROMUM ROQSQ"; break;
			case 'g' : case 'G' : return " 15MXVNTMRMPNOPOSPURVTVVUVR RSRVR"; break;
			case 'h' : case 'H' : return "  9MWOMOV RUMUV ROQUQ"; break;
			case 'i' : case 'I' : return "  3PTRMRV"; break;
			case 'j' : case 'J' : return "  7NUSMSTRVPVOTOS"; break;
			case 'k' : case 'K' : return "  9MWOMOV RUMOS RQQUV"; break;
			case 'l' : case 'L' : return "  6MVOMOV ROVUV"; break;
			case 'm' : case 'M' : return " 12LXNMNV RNMRV RVMRV RVMVV"; break;
			case 'n' : case 'N' : return "  9MWOMOV ROMUV RUMUV"; break;
			case 'o' : case 'O' : return " 14MXRMPNOPOSPURVSVUUVSVPUNSMRM"; break;
			case 'p' : case 'P' : return " 10MWOMOV ROMSMUNUQSROR"; break;
			case 'q' : case 'Q' : return " 17MXRMPNOPOSPURVSVUUVSVPUNSMRM RSTVW"; break;
			case 'r' : case 'R' : return " 13MWOMOV ROMSMUNUQSROR RRRUV"; break;
			case 's' : case 'S' : return " 13MWUNSMQMONOOPPTRUSUUSVQVOU"; break;
			case 't' : case 'T' : return "  6MWRMRV RNMVM"; break;
			case 'u' : case 'U' : return "  9MXOMOSPURVSVUUVSVM"; break;
			case 'v' : case 'V' : return "  6MWNMRV RVMRV"; break;
			case 'w' : case 'W' : return " 12LXNMPV RRMPV RRMTV RVMTV"; break;
			case 'x' : case 'X' : return "  6MWOMUV RUMOV"; break;
			case 'y' : case 'Y' : return "  7MWNMRQRV RVMRQ"; break;
			case 'z' : case 'Z' : return "  9MWUMOV ROMUM ROVUV" ; break;

			case '0' : return " 12MWRMPNOPOSPURVTUUSUPTNRM" ; break;
			case '1' : return "  4MWPORMRV" ; break;
			case '2' : return "  9MWONQMSMUNUPTROVUV" ; break;
			case '3' : return " 15MWONQMSMUNUPSQ RRQSQURUUSVQVOU"; break;
			case '4' : return "  7MWSMSV RSMNSVS" ; break;
			case '5' : return " 14MWPMOQQPRPTQUSTURVQVOU RPMTM" ; break;
			case '6' : return " 14MWTMRMPNOPOSPURVTUUSTQRPPQOS" ; break;
			case '7' : return "  6MWUMQV ROMUM" ; break;
			case '8' : return " 19MWQMONOPQQSQUPUNSMQM RQQOROUQVSVUUURSQ" ; break;
			case '9' : return " 14MWUPTRRSPROPPNRMTNUPUSTURVPV" ; break;

			
			
			case '.': return "  6PURURVSVSURU";//210
			case ',': return "  7PUSVRVRUSUSWRY";//211
			case ':': return " 12PURPRQSQSPRP RRURVSVSURU";//212
			case ';': return " 13PURPRQSQSPRP RSVRVRUSUSWRY";//213
			case '!': return " 12PURMRR RSMSR RRURVSVSURU";//214
			case '?': return " 17NWPNRMSMUNUPRQRRSRSQUP RRURVSVSURU";//215
			case '\'':return "  3PTRMRQ";//216
			case '\"':return "  6NVPMPQ RTMTQ";//217
			case '/': return "  3MWVLNW";//220
			case '(': return "  7OVTLRNQPQSRUTW";//221
			case ')': return "  7NUPLRNSPSSRUPW";//222
			case '|': return "  3PTRLRW";//223
			case '#': return " 12MXRLPW RULSW ROPVP ROSVS";//233
			case '*': return "  9JZRLRX RMOWU RWOMU";//728
			case '=': return "  6LXNPVP RNTVT";//226
			case '-': return "  3KYKRYR";//806
			case '_': return "  3JZJZZZ";//998
			case '[': return " 12MWPHP\\ RQHQ\\ RPHUH RP\\U\\";//1223
			case ']': return " 12MWSHS\\ RTHT\\ ROHTH RO\\T\\";//1224
			case '{': return " 38LWSHQIPJPLRNSP RQIPL RSNRQ RPJQLSNSPRQPRRSSTSVQXPZ RRSSV RPXQ[ RSTRVPXPZQ[S\\"; 
			case '}': return " 38MXQHSITJTLRNQP RSITL RQNRQ RTJSLQNQPRQTRRSQTQVSXTZ RRSQV RTXS[ R QTRVTXTZS[Q\\";
			default: return NULL;
			}
		};
	
	
static void charToPathOp(vector<Operator>& operations,int letter)
		{
		size_t i;
		operations.clear();
		if(letter==' ') return ;
		
		if(s==NULL)
			{
			return;
			}

		int num_vertices=0;
		for( i=0;i< 3;++i)
			{
			char c=s[i];
			if(c==' ' || c=='\n') continue;
			num_vertices = num_vertices*10+(c-'0');
			}
		num_vertices--;
		i+=2;
		int nop=0;
		
		while(nop<num_vertices)
			{
			Operator pathOp;

			pathOp.operator=operations.empty()? MOVETO: LINETO;
			char c=s[i++];
			if(c==' ')
				{
				c=s[i++];
				if(c!='R') {fprintf(stderr,"ERROR");break;}//ERROR!
				nop++;
				pathOp.operator= MOVETO;
				c=s[i++];
				}
			pathOp.x=c-'R';
			c=s[i++];
			pathOp.y=c-'R';
			nop++;
			
			operations.push_back(pathOp);
			}
		};



Canvas::Canvas(int w, int h),_width(width),_height(height) {
	State* st=new State;
	st->stroke = &Color::black;
	st->fill = NULL;
	states.push_back(st);
	}
	
Canvas::~Canvas() {
while(!states.empty()) {
	delete states.pop_back();
	}
}
int Canvas::width() const {return this->_width;}
int Canvas::height() const {return this->_height;}
void Canvas::push() {
	State* prev= states.top();
	State* st=new State;
	st->stroke = prev->stroke;
	st->fill = prev->fill;
	states.push_back(st);
	}
void Canvas::pop() {
	delete states.pop_back();
	}

void Canvas::noStroke() {
	State* st=states.top();
	if(st->stroke!=NULL) delete st->stroke;
	st->stroke=NULL;
	}
	
void Canvas::noFill() {
	State* st=states.top();
	if(st->fill!=NULL) delete st->fill;
	st->fill=NULL;
	}
void Canvas::stroke(Color c) {
	noStroke();
	states.top()->stroke=new Color(c);
	}
void Canvas::fill(Color c) {
	noFill();
	states.top()->fill=new Color(c);
	}

void Canvas::line(double x1,double y1,double x2,double y2) {
	line(Line2D(x1,y2,x2,y2));
	}

void Canvas::hershey(const char* s,Rectangle2D r) {
	hershey(s,r.x,r.y,r.width,r.height);
	}

void Canvas::hershey(
	const char* s,
	double x, double y,
	double width, double height
	)
	{
	double
	size_t s_length,i,n;
	if(s==NULL || width==0 || height==0) return;
	s_length=strlen(s);
	if(s_length==0) return;
	

	double dx=width/s_length;
	for(i=0;i < s_length;++i)
		{
		Polygon2D polygon;
		vector<Operator> operations;
		charToPathOp(operations,s[i]);
		for(n=0;n< operations.size();++n)
			{
			OperatorPtr p2= operations[n];
			double x2= x+ (p2->x/ptr->scalex)*dx + dx*i +dx/2.0;
			double y2= y+ (p2->y/ptr->scaley)*height +height/2.0 ;
			
			if(p2->operator == LINETO)
				{
				polygon.add(x2,y2);
				}
			else
				{
				if(!polygon.emoty()) polyline(polygon);
				polygon.clear();
				polygon.add(x2,y2);
				}
			}
		}
	}
