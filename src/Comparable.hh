#ifndef COMPARABLE_HH
#define COMPARABLE_HH
template<typename T>
class Comparable {
	public:
		virtual int compareTo(const T& cp) const=0;
	};


#define MAKE_DEFAULT_COMPARABLE_METHODS(CLAZZ) \
	virtual bool operator < (const CLAZZ& cp) const { return this->compareTo(cp) < 0; } \
	virtual bool operator <= (const CLAZZ& cp) const { return this->compareTo(cp) <= 0; } \
	virtual bool operator > (const CLAZZ& cp) const { return this->compareTo(cp) > 0; } \
	virtual bool operator >= (const CLAZZ& cp) const { return this->compareTo(cp) >= 0; } \
	virtual bool operator == (const CLAZZ& cp) const { return this->compareTo(cp) == 0; } \
	virtual bool operator != (const CLAZZ& cp) const { return this->compareTo(cp) != 0; }

#endif
