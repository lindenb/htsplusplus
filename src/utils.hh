#ifndef UTILS_HH
#define UTILS_HH
#include <cstdlib>
#include <cstdio>
#include <memory>

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


template<typename T>
class Pointer {
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


class StringUtils {
	public:
		static bool isBlank(const char* s);
	};



class IoUtils {
	public:
		/* The system-dependent default name-separator character, represented as a string for convenience. */
		static const char* separator();
		/* The system-dependent default name-separator character. */
		static char separatorChar();
		static const bool assertFileExist(const char* f);
		static std::unique_ptr<FILE,stdio_file_closer> fopen(const char* f, const char* mode);
		static std::string slurpFile(const char* filename);
	};

#endif

