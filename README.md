# hts+++

2021 March 2

## subcommands

 + **bcfprune** Select variants  from BCF/VCF spaced by a distance 'd'.

# Compilation

```
git clone "https://github.com/lindenb/htsplusplus"
cd htsplusplus
# set the path to a compiled C htslib directory
make HTSLIB=/path/to/htslib
```

# Example

```
export LD_LIBRARY_PATH=/path/to/htslib
./hts++ bcfprune -d 100 test.bcf
```

## bcfprune

# Usage

```
Usage: ./hts++ bcfprune [ -O (o|v|z) ] [-o fileout] -d <distance> (stdin|bcf)
Options:
  -h print help
  -d (int) distance
  -o (file) output file (default stdout)
  -O (char) output format z:gzip vcf v:vcf b:bcf (default v)
```

# Author

Pierre Lindenbaum Phd 2020.

