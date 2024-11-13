#ifndef IO_HH
#define IO_HH

class Closeable {
	public:
		virtual void close()= 0;
		virtual ~Closeable() {close();}
	};



class Reader : public Closeable {
	public:
		virtual int peek()=0;
		virtual int getc()=0;
		virtual size_t read(void* ptr,size_t size)=0;
		virtual void mustRead(void* ptr,size_t size) {
			if(read(ptr,size)!=size)
			}

#define READ_FUN(TYPE,NAME) virtual TYPE NAME() {\
			TYPE v;\
			mustRead((void*)&v,sizeof(TYPE));\
			return v;\
			}

		READ_FUN(int32_t,readInt32);
		READ_FUN(int64_t,readInt64);
		READ_FUN(float,readFloat);
		READ_FUN(double,readDouble);
		std::auto_ptr<std::string> readline() {
			std::string* line=NULL;
			for(;;) {
				int c = getc();
				if(c==EOF || c=='\n') break;
				if(line==NULL) line=new std::string;
				*(line)+=(char)c;
				}
			return std::auto_ptr<std::string>(line);
			}

#undef READ_FUN
	}

class FileReader : public Reader {
	protected:
		int _fd;
	public:
		FileReader(const char* s) {
			}
		virtual ~FileReader() {
			}
		
	}


#endif

