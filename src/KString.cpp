#include "KString.hh"

using namespace std;
using namespace htspp;

KString::KString():ks(KS_INITIALIZE) {
	}

KString::KString(const char* s):ks(KS_INITIALIZE) {
	::kputs(s,&ks);	
	}

KString::KString(const KString& cp):ks(KS_INITIALIZE) {
	::kputsn(cp.c_str(),cp.size(),&ks);	
	}

KString::~KString() {
	::ks_free(&ks);
	}
	
kstring_t* KString::get() {
	return  &ks;
	}

char KString::operator[](int i) const {
    return c_str()[i];
    }

const char* KString::c_str() const {
    return ::ks_c_str(const_cast<kstring_t*>(&ks));
    }

std::ostream& KString::print(std::ostream& out) const{
	 out.write(c_str(),size());
   return out;
    }
    
    
KString& KString::reserve(std::size_t n) {
	::ks_resize(&ks,n);
	return *this;
	}

size_t KString::size() const {
	return ks_len(const_cast<kstring_t*>(&ks));
}

size_t KString::length() const {
	return size();
}

KString& KString::clear() {
	::ks_clear(get());
	return *this;
}


KString& KString::append(const char* s) {
	::kputs(s,&ks);
	return *this;
}

KString& KString::append(char c) {
	::kputc(c,get());
	return *this;
}

KString& KString::append(const char* s,size_t n) {
	::kputsn(s,n,get());
	return *this;
}

