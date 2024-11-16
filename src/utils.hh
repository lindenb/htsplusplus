#ifndef UTILS_HH
#define UTILS_HH
#include <cassert>
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
		virtual T* get() const {
			return ptr;
			}

		virtual bool is_null() const {
			return get()==NULL;
			}
		virtual T* require_not_null() const {
		  //TODO check null
		  return get();
			}
		virtual bool is_not_null() const {
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

