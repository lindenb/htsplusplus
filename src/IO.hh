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

