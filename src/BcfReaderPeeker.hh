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

