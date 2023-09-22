#ifndef BCF_READER_PEEKER_H
#define BCF_READER_PEEKER_H

#include <vector>
#include "BcfReader.hh"

class BcfReaderPeeker {
    private:
        BcfReader* reader;
        std::vector<BcfRecordHeader*> pool;
        unsigned long read_index;
        void reserve(unsigned int n) {
            while(pool.size() < n) {
                pool.push_back(new BcfRecordHeader);
                }
            }
    public:
        BcfReaderPeeker(BcfReader* reader):reader(reader),read_index(0UL) {
            }
        virtual ~BcfReaderPeeker() {
            for (auto p : pool) {
                delete p;                
                }
            }
        virtual BcfRecordHeader* peek() {
            if(read_index==0UL) {
               reserve(1L);
               if(reader->read2(pool[0])) {
                    read_index++;
                    }
                else
                    {
                    return NULL;
                    }               
               }
            return pool[0];
            }

        virtual BcfRecordHeader* read() {
            if(read_index==1UL) {
                read_index=0UL;
                return pool[0];
                }
            else
                {
                reserve(1L);
                if(reader->read2(pool[0])) {
                    read_index++;
		    return pool[0];
                    }
                else
                    {
                    return NULL;
                    }
                }
            }
    };


#endif

