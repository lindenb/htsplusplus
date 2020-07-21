# bcfprune

2020 July 20. Pour Isabel Alves. Selection de variants espacés par une distance 'd'.


# Usage

```
Usage: ./bcfprune [ -O (o|v|z) ] [-o fileout] -d <distance> (stdin|bcf)
Options:
  -h print help
  -d (int) distance
  -o (file) output file (default stdout)
  -O (char) output format z:gzip vcf v:vcf b:bcf (default v)
```

# Compilation

```
git clone "https://LOGIN@gitlab.univ-nantes.fr/pierre.lindenbaum/bcfprune.git"
cd bcfprune
make HTSLIB=/sandbox/apps/bioinfo/binaries/htslib/0.0.0/htslib
```

# Example

```
export LD_LIBRARY_PATH=/sandbox/apps/bioinfo/binaries/htslib/0.0.0/htslib
./bcfprune -d 100 test.bcf
```

# Author

Pierre Lindenbaum Phd 2020.

