#include "Faidx.hh"

std::unique_ptr<Faidx> Faidx::load(const char* filename) {
	ASSERT_NOT_NULL(filename);
	faidx_t *f = fai_load(filename);
	ASSERT_NOT_NULL(f);
	return std::unique_ptr<Faidx>(new Faidx(f));
	}