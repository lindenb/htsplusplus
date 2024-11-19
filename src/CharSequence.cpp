#include <sstream>
#include "CharSequence.hh"


using namespace htspp;
using namespace std;

CharSequence::CharSequence() {
}
 
CharSequence::~CharSequence() {
}


bool CharSequence::empty() const {
return size()==0;

}

std::ostream& CharSequence::print(std::ostream& out) const {
for(size_t i=0;i< size();i++) out << at(i);
return out;
}


char CharSequence::operator[](int i) const {
    return at(i);
    }

std::size_t CharSequence::size() const {
return length();
}

std::string CharSequence::to_string() const {
std::ostringstream os;
print(os);
return os.str();
}

