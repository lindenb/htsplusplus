#ifndef OPTIONAL_HH
#define OPTIONAL_HH

namespace htspp {

template<typename T>
class BaseOptional {
private:
	T value;
	bool defined;
protected:
	BaseOptional(T value,bool defined) {
		}
public:
	virtual bool isPresent() {
		return defined;
		}
	virtual T get() {
		return value;
		}
};



#define MAKE_OPTIONAL_CLASS(CLASS,DATATYPE) \
class CLASS:public BaseOptional<DATATYPE> {\
private:\
	CLASS(DATATYPE value,bool defined):BaseOptional(value,defined) {}\
public:\
	static CLASS empty();\
	static CLASS of(DATATYPE v);\
};

MAKE_OPTIONAL_CLASS(OptionalInt,int)
MAKE_OPTIONAL_CLASS(OptionalDouble,double)


#undef MAKE_OPTIONAL_CLASS

}

#endif

