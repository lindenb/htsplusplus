#include <iterator>
#include "BcfReader.hh"
#include "BcfWriter.hh"
#include "programs.hh"
#include "utils.hh"

using namespace std;


template<typename some_iterator>
static int run(BcfconcatArgs& args, some_iterator r, some_iterator r_end) {
    std::unique_ptr<BcfWriter> out;
    std::vector<std::string> samples;
    while(r!=r_end) {
        std::string fname = (*r);
        if(StringUtils::isBlank(r->c_str())) {
           ++r;
           continue;
           }
        std::unique_ptr<BcfReader> in = BcfReader::open(r->c_str());
        if(!out) {
                ASSERT_NOT_NULL(in->header);
                BcfWriterFactory writer_factory;
                out = writer_factory.format(args.bcf_output_format).open(args.output_filename);
                out->write_header(in->header->get());
                samples = in->header->samples();
                }
        else
                {
                std::vector<std::string> samples2 =  in->header->samples();
                if(samples!=samples2) {
                    THROW_ERROR("samples are not the same from one vcf to another ("<< *r << ")");                    
                    }
                }
        BcfRecordHeader bcf;
        while(in->read2(&bcf)) {
            out->write1(in->header->get(),bcf.get());            
            }

        ++r;        
        }
    if(!out) {
        ERROR("no vcf was read");
        return EXIT_FAILURE;
        }
    return 0;
    }

int main_bcfconcat(int argc, char** argv) {
BcfconcatArgs args;
args.parse(argc,argv);
PROGRAM_COMMON(args);
int ret;
if(optind==argc || (optind+1==argc && strcmp(argv[optind],"-")==0) )  {
    auto r=std::istream_iterator<IsLineIterator>(cin);
    auto r_end =std::istream_iterator<IsLineIterator>();
    ret = run(args,r,r_end);
    }
else
    {
    std::vector<std::string> paths =   IoUtils::unroll(argc,argv,optind);
    auto r= paths.begin();
    auto r_end = paths.end();
    ret = run(args,r,r_end);
    }
return ret;
}
