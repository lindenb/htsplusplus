
# hts++


# Compilation

compilation requires a C++ compiler and `xsltproc`.

```
git clone "https://github.com/lindenb/htsplusplus"

autoconf -Wno-syntax
./configure --with-htslib= --with-htslib=${PWD}/../htslib    # see ./configure --help and samtools's INSTALL
make

```




# subcommands



##  SAMVIEWWITHMATE

select read and their mate that falls within a region from a **query sorted** **BAM**

### Usage

```
hts++ samviewwithmate [options...] (files)
```


### Options

| short-option | long-option | argument | description | default-value |
|--------------|-------------|----------|-------------|---------------|
| -h | --help |  | Display help and exit |  |
| -v | --version |  | Display Version and exit |  |
| -o | --output |   <ARG>   | output or stdout |  |
| -R | --reference |   <fasta>   | FASTA reference |  |
|  | --compression-level |   <0-9>   | compression-level 0 (low compression) to 9 (best compression) |  |
| -O | --bam-output-format |   <format>   | output format for sam: SAM, BAM, CRAM |  |
| -@ | --nthreads |   <nthreads>   | Number of threads |  |
| -i | --inverse |  | inverse logic: keep read where no read in the pair overlap the BED file |  |
| -B | --bed |   <BED>   | A BED file. |  |




##  BCFPRUNE

Select variants with a fixed distance in a **VCF** file.

### Usage

```
hts++ bcfprune [options...] (files)
```


### Options

| short-option | long-option | argument | description | default-value |
|--------------|-------------|----------|-------------|---------------|
| -h | --help |  | Display help and exit |  |
| -v | --version |  | Display Version and exit |  |
| -o | --output |   <ARG>   | output or stdout |  |
|  | --compression-level |   <0-9>   | compression-level 0 (low compression) to 9 (best compression) |  |
| -O | --vcf-output-format |   <format>   | output format for VCF/BCF: z:gzip vcf v:vcf b:bcf (default v) |  |
| -d | --distance |   <ARG>   | distance |  |




##  BCFCONCAT

concatenanate multiple vcf/bcf files

### Usage

```
hts++ bcfconcat in1.bcf in2.vcf.gz in3.vcf
hts++ bcfconcat bcfs.list
cat bcfs.list | hts++ bcfconcat
```


### Options

| short-option | long-option | argument | description | default-value |
|--------------|-------------|----------|-------------|---------------|
| -h | --help |  | Display help and exit |  |
| -v | --version |  | Display Version and exit |  |
| -o | --output |   <ARG>   | output or stdout |  |
|  | --compression-level |   <0-9>   | compression-level 0 (low compression) to 9 (best compression) |  |
| -O | --vcf-output-format |   <format>   | output format for VCF/BCF: z:gzip vcf v:vcf b:bcf (default v) |  |




##  BCFFILTERGT

Select variant using an expression

### Usage

```
hts++ bcffiltergt [options...] (files)
```


### Options

| short-option | long-option | argument | description | default-value |
|--------------|-------------|----------|-------------|---------------|
| -h | --help |  | Display help and exit |  |
| -v | --version |  | Display Version and exit |  |
| -o | --output |   <ARG>   | output or stdout |  |
|  | --compression-level |   <0-9>   | compression-level 0 (low compression) to 9 (best compression) |  |
| -O | --vcf-output-format |   <format>   | output format for VCF/BCF: z:gzip vcf v:vcf b:bcf (default v) |  |
| -F | --soft-filter |   <FILTER-ID>   | soft FILTER name. (default: filter out variants) |  |
| -e | --expression |   <expression>   | Expression as string |  |
| -f | --file-expression |   <FILE>   | Expression as file |  |




##  BCFHEAD

Show the first 'N' variants in a VCF

### Usage

```
hts++ bcfhead [options...] (files)
```


### Options

| short-option | long-option | argument | description | default-value |
|--------------|-------------|----------|-------------|---------------|
| -h | --help |  | Display help and exit |  |
| -v | --version |  | Display Version and exit |  |
| -o | --output |   <ARG>   | output or stdout |  |
|  | --compression-level |   <0-9>   | compression-level 0 (low compression) to 9 (best compression) |  |
| -O | --vcf-output-format |   <format>   | output format for VCF/BCF: z:gzip vcf v:vcf b:bcf (default v) |  |
| -n | --count |   <COUNT>   | Number of variants |  |




##  BCF2INTERVALS

convert VCF/BCF to interval of variants that are distant from 'x' bases.

### Usage

```
hts++ bcf2intervals [options...] (files)
```


### Options

| short-option | long-option | argument | description | default-value |
|--------------|-------------|----------|-------------|---------------|
| -h | --help |  | Display help and exit |  |
| -v | --version |  | Display Version and exit |  |
| -o | --output |   <ARG>   | output or stdout |  |
| -d | --distance_per_interval |   <ARG>   | distance |  |
| -m | --min-distance |   <ARG>   | min distance |  |




##  SAMRMDUPNAMES

remove duplicated names in sorted BAM (prevent graphtyper from failing)

### Usage

```
hts++ samrmdupnames [options...] (files)
```


### Options

| short-option | long-option | argument | description | default-value |
|--------------|-------------|----------|-------------|---------------|
| -h | --help |  | Display help and exit |  |
| -v | --version |  | Display Version and exit |  |
| -o | --output |   <ARG>   | output or stdout |  |
| -R | --reference |   <fasta>   | FASTA reference |  |
|  | --compression-level |   <0-9>   | compression-level 0 (low compression) to 9 (best compression) |  |
| -O | --bam-output-format |   <format>   | output format for sam: SAM, BAM, CRAM |  |
| -@ | --nthreads |   <nthreads>   | Number of threads |  |




##  FASTQSPLITNFILES

Split FASTQ to N files

### Usage

```
hts++ fastqsplitnfiles [options...] (files)
```


### Options

| short-option | long-option | argument | description | default-value |
|--------------|-------------|----------|-------------|---------------|
| -h | --help |  | Display help and exit |  |
| -v | --version |  | Display Version and exit |  |
| -o | --output |   <ARG>   | output or stdout |  |
| -p | --prefix |   <ARG>   | output prefix |  |
| -n | --count-files |   <ARG>   | number of files |  |
|  | --compression-level |   <0-9>   | compression-level 0 (low compression) to 9 (best compression) |  |




##  BCFSTREAMUNTIL

Stream a VCF until a position is found.

### Usage

```
hts++ bcfstreamuntil [options...] (files)
```


### Options

| short-option | long-option | argument | description | default-value |
|--------------|-------------|----------|-------------|---------------|
| -h | --help |  | Display help and exit |  |
| -v | --version |  | Display Version and exit |  |
|  | --compression-level |   <0-9>   | compression-level 0 (low compression) to 9 (best compression) |  |
| -O | --vcf-output-format |   <format>   | output format for VCF/BCF: z:gzip vcf v:vcf b:bcf (default v) |  |
| -o | --output |   <ARG>   | output or stdout |  |
| -p | --position |   <ARG>   | stop after this position. syntax 'CHROM:POS' |  |




##  TESTS

peforms some tests

### Usage

```
hts++ tests [options...] (files)
```


### Options

| short-option | long-option | argument | description | default-value |
|--------------|-------------|----------|-------------|---------------|
| -h | --help |  | Display help and exit |  |
| -v | --version |  | Display Version and exit |  |
| -D | --directory |   <DIR>   | test directory |  |




# Author

Pierre Lindenbaum Phd 2022.
