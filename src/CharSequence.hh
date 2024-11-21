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

#ifndef CHAR_SEQUENCE_HH
#define CHAR_SEQUENCE_HH

#include <memory>
#include <iostream>
#include <string>
#include <vector>
namespace htspp {

class CharSequence {
    public:
          typedef std::size_t size_t; 
          CharSequence();
          virtual ~CharSequence();
          virtual bool empty() const;
          virtual char at(int i) const=0;
		  virtual char operator[](int i) const;
		  virtual std::ostream& print(std::ostream& out) const;
          virtual size_t size() const;
          virtual size_t length() const=0;
          virtual std::string to_string() const;
          virtual bool starts_with(const char* s) const;
          
          
          virtual std::unique_ptr<CharSequence> toupper() const;
          virtual std::unique_ptr<CharSequence> tolower() const;
          virtual std::unique_ptr<CharSequence> clone() const;
          
          virtual std::vector<std::unique_ptr<CharSequence> > split(char c) const;
          virtual std::vector<std::unique_ptr<CharSequence> >::size_type split(char c,std::vector<std::unique_ptr<CharSequence> > & v) const;
          virtual std::unique_ptr<CharSequence> substring(CharSequence::size_t start) const;
          virtual std::unique_ptr<CharSequence> substring(CharSequence::size_t start,CharSequence::size_t end ) const;

    };

}

#endif

