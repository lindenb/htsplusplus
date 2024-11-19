#ifndef CHAR_SEQUENCE_HH
#define CHAR_SEQUENCE_HH

#include <iostream>
#include <string>
namespace htspp {

class CharSequence {
    public:
          CharSequence();
          virtual ~CharSequence();
          virtual bool empty() const;
          virtual char at(int i) const=0;
		  virtual char operator[](int i) const;
		  virtual std::ostream& print(std::ostream& out) const;
          virtual std::size_t size() const;
          virtual std::size_t length() const=0;
          virtual std::string to_string() const;

    };

}

#endif

