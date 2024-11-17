#include "CigarOperator.hh"
#include "debug.hh"

using namespace htspp;

    /** Match or mismatch */
const CigarOperator* CigarOperator::M = new CigarOperator(true, true,   'M');
    /** Insertion vs. the reference. */
const CigarOperator* CigarOperator::I = new CigarOperator(true, false,  'I');
    /** Deletion vs. the reference. */
const CigarOperator* CigarOperator::D = new CigarOperator(false, true,  'D');
    /** Skipped region from the reference. */
const CigarOperator* CigarOperator::N = new CigarOperator(false, true,  'N');
    /** Soft clip. */
const CigarOperator* CigarOperator::S = new CigarOperator(true, false,  'S');
    /** Hard clip. */
const CigarOperator* CigarOperator::H = new CigarOperator(false, false, 'H');
    /** Padding. */
const CigarOperator* CigarOperator::P = new CigarOperator(false, false, 'P');
    /** Matches the reference. */
const CigarOperator* CigarOperator::EQ = new CigarOperator(true, true,  '=');
    /** Mismatches the reference. */
const CigarOperator* CigarOperator::X = new CigarOperator(true, true,   'X');


CigarOperator::CigarOperator(bool consumesReadBases, bool consumesReferenceBases, char c):
	_consumesReadBases(consumesReadBases),
	_consumesReferenceBases(consumesReferenceBases),
	_character(c) {
 }

char CigarOperator::getc() const { return _character;}
bool CigarOperator::consumesReadBases() const { return _consumesReadBases;}
bool CigarOperator::consumesReferenceBases() const { return _consumesReferenceBases;}



const CigarOperator* CigarOperator::of(char c) {
	switch(c) {
		    case 'M':
            return CigarOperator::M;
        case 'I':
            return CigarOperator::I;
        case 'D':
            return CigarOperator::D;
        case 'N':
            return CigarOperator::N;
        case 'S':
            return CigarOperator::S;
        case 'H':
            return CigarOperator::H;
        case 'P':
            return CigarOperator::P;
        case '=':
            return CigarOperator::EQ;
        case 'X':
            return CigarOperator::X;
        default:
			THROW_ERROR("Bad cigar char " << c);
		}
	}

bool CigarOperator::consumesReadBases(char c) {
    return of(c)->CigarOperator::consumesReadBases();
    }
bool CigarOperator::consumesReferenceBases(char c)  {
    return of(c)->CigarOperator::consumesReferenceBases();
    }
   
bool CigarOperator::is_clip() const {
	switch(getc()) {
		case 'H':case 'S': return true;
		default: return false;
		}
	}   
   
bool CigarOperator::is_align() const {
	switch(getc()) {
		case 'M':case 'X':  case '=': return true;
		default: return false;
		}
	}   
bool CigarOperator::is_del() const {
	switch(getc()) {
		case 'D':case 'N':  case '=': return true;
		default: return false;
		}
	}
bool CigarOperator::is_ins() const {
	switch(getc()) {
		case 'I':  case '=': return true;
		default: return false;
		}
	}
bool CigarOperator::is_indel() const {
	return is_del() || is_ins();
	}
  