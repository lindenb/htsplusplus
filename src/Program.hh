#ifndef PROGRAM_HH
#define PROGRAM_HH
#include <string>
#include "GetOpt.hh"
namespace htspp {
class Program {
    protected:
        std::string _name;
        std::string _description;
    public:
        Program(const char* short_name,const char* description);
        const char* name() const;
        const char* description() const;
        virtual ~Program();
        virtual int main(int argc,char** argv)=0;
        virtual bool is_hidden() const;
        virtual void usage(std::ostream& out,GetOpt* getopt);
};

}

#endif

