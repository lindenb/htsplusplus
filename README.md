# bcfprune

2020 July 20.

Select variants  from BCF/VCF spaced by a distance 'd'.

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
git clone "https://github.com/lindenb/bcfprune"
cd bcfprune
# set the path to a compiled C htslib directory
make HTSLIB=/path/to/htslib
```

# Example

```
export LD_LIBRARY_PATH=/path/to/htslib
./bcfprune -d 100 test.bcf
```

# Author

Pierre Lindenbaum Phd 2020.

