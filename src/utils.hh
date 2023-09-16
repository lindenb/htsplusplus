#ifndef UTILS_HH
#define UTILS_HH

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

class StringUtils {
	};

class IoUtils {
	public:
		/* The system-dependent default name-separator character, represented as a string for convenience. */
		static const char* separator();
		/* The system-dependent default name-separator character. */
		static char separatorChar();
		static const bool assertFileExist(const char* f);
	};

#endif

