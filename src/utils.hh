#ifndef UTILS_HH
#define UTILS_HH
#include <cstdlib>
#include <cstdio>
#include <memory>
#include <vector>
#include <string>
#include <htslib/hts.h>
#include "Path.hh"

/** prevent java syntax from failing ...*/
#define final
#define boolean bool
#define null NULL
#define abstract virtual

#define FLAG_NULL_ON_ERROR 1

namespace htspp {

struct stdio_file_closer
	{
    void operator()(std::FILE* fp) const {
    	if(fp!=NULL) {
    		std::fclose(fp);
    		}
    	}
	};

template<typename T>
struct stdlib_free
	{
    void operator()(T* fp) const {
    	if(fp!=NULL) {
    		std::free((void*)fp);
    		}
    	}
	};


class Object {
	public:
		Object() {}
		virtual ~Object() {}
	};

template<typename T>
class Pointer : public Object {
	public:
		T* ptr;
		Pointer():ptr(NULL) {
			}
		Pointer(T* ptr):ptr(ptr) {
			}
		virtual ~Pointer() {
			}
		virtual T* get() {
			return ptr;
			}
		virtual bool is_null() {
			return get()==NULL;
			}
		virtual bool is_not_null() {
			return !this->is_null();
			}
		void set_null() {
			ptr=NULL;
			}
		};

template<typename T>
class Iterator {
	public:
		Iterator() {
			}
		virtual ~Iterator() {
			}
		virtual bool hasNext()=0;
		T next()=0;
		void close() {
			}
		unsigned long count() {
			unsigned long n=0UL;
			while(hasNext()) {
				next();
				n++;
				}
			close();
			return n;
			}
	};

// https://stackoverflow.com/questions/2291802/is-there-a-c-iterator-that-can-iterate-over-a-file-line-by-line
class IsLineIterator : public std::string 
    { 
    friend std::istream & operator>>(std::istream& is, IsLineIterator& line)
        {   
            return std::getline(is, line);
        }
    };

template<typename T>
class AbstractList {
	public:
		typedef unsigned int size_type;
		AbstractList<T>() {}
		virtual ~AbstractList() {}
		virtual AbstractList::size_type size()=0;
		virtual T get( AbstractList::size_type idx)=0;
	private:
		class MyIterator:public Iterator<T> {
			private:
				AbstractList<T>* owner;
				AbstractList<T>::size_type idx;
			public:
				MyIterator(AbstractList<T>* owner):owner(owner),idx(0UL) {}
				virtual ~MyIterator() {}
				bool hasNext() { return idx < owner->size();}
				T next() {  return owner->get(idx++);}
			};
	public:
		std::unique_ptr<Iterator<T> > iterator() {
			return std::unique_ptr<Iterator<T> >(new MyIterator(this));
			}
	};

class CharSplitter {
	private:
		std::string delims;
	protected:
		virtual bool match(char c) const {
			return  delims.find(c)!=std::string::npos ;
			}
	public:
		CharSplitter(char delim):delims() {
			delims+=delim;
			}
		CharSplitter(std::string delims):delims(delims) {
			}
		
		std::vector<std::string>::size_type split(std::string s,std::vector<std::string>& v,int limit) const {
			std::string::size_type prev=0UL,i=0UL, len = s.size();
			//remove trailing
			while(len>0UL && match(s[len-1])) {
				len--;
				}
			if(limit==1) {
				v.push_back(s.substr(0,len));
				return v.size();
				}
			for(;;) {
				if(i>=len || match(s[i])) {
					v.push_back(s.substr(prev,i-prev));
					if(i>=len) break;
					prev=i+1;
					if(limit>1 && v.size()+1==(unsigned int)limit) {
						v.push_back(s.substr(prev,len-prev));
						return v.size();
						}
					}
				i++;
				}
			return v.size();
			}
		
		std::vector<std::string>::size_type split(std::string s,std::vector<std::string>& v) const {
			return split(s,v,-1);
			}
		
		std::vector<std::string> split(std::string s) const {
			std::vector<std::string>  v;
			split(s,v);
			return v;
			}
		std::vector<std::string> split(std::string s,int limit) const {
			std::vector<std::string>  v;
			split(s,v,limit);
			return v;
			}
		static CharSplitter TAB;
		static CharSplitter COMMA;
	};



class CoordMath {
	public:
		static hts_pos_t getLength(hts_pos_t start, hts_pos_t end);
		static bool overlaps(hts_pos_t start, hts_pos_t end, hts_pos_t start2,hts_pos_t end2);
	};

class StringUtils {
	public:
	static bool isBlank(const char* s);
        static bool startsWith(const char* s,const char* prefix);
        static bool endsWith(const char* s,const char* suffix);
	};



class IoUtils {
	public:
		/* The system-dependent default name-separator character, represented as a string for convenience. */
		static const char* separator();
		/* The system-dependent default name-separator character. */
		static char separatorChar();
		static const bool assertFileExist(const char* f);
		static FILE* fopen(const char* f, const char* mode);
		//static std::unique_ptr<std::ostream> output_or_stdout(const char* f);
		static std::string slurpFile(const char* filename);
        static std::vector<std::string> unroll(int argc,char** argv,int optind); 
	};


}
#endif

