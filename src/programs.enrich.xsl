<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
	version="1.0"
	>
<xsl:output method="xml" indent="yes"/>

<xsl:template match="/">
<xsl:apply-templates select="*"/>
</xsl:template>

<xsl:template match="macro[@name='what.is.bed']">
	<xsl:text>A BED file.</xsl:text>
</xsl:template>

<xsl:template match="macro[@name='help']">
		<option name="show_help" type="bool" short-option="h" long-option="help">
			<short-description>Display help and exit</short-description>
		</option>
</xsl:template>

<xsl:template match="macro[@name='version']">
		<option name="show_version" type="bool" short-option="v" long-option="version">
			<short-description>Display Version and exit</short-description>
		</option>
</xsl:template>


<xsl:template match="macro[@name='output']">
                <option name="output_filename" type="string" short-option="o" long-option="output">
                        <short-description>output or stdout</short-description>
                </option>
</xsl:template>

<xsl:template match="macro[@name='compression_level']">
                <option name="compression_level" type="int" long-option="compression-level" default-value="5" arg-name="0-9">
                        <short-description>compression-level 0 (low compression) to 9 (best compression)</short-description>
                </option>
</xsl:template>


<xsl:template match="macro[@name='reference']">
		<option name="reference" type="string" short-option="R" long-option="reference" arg-name="fasta">
			<xsl:if test="@required='true'">
			<xsl:attribute name="required">true</xsl:attribute>
			</xsl:if>
			<short-description>FASTA reference</short-description>
		</option>
</xsl:template>

<xsl:template match="macro[@name='nthreads']">
		<option name="nthreads" type="int" short-option="@" long-option="nthreads" arg-name="nthreads" default="-1">
			<short-description>Number of threads</short-description>
		</option>
</xsl:template>

<xsl:template match="macro[@name='bcf.output']">
	<macro name="compression_level"/>
	<option name="bcf_output_format" type="char" short-option="O" long-option="vcf-output-format" arg-name="format" default-value="'v'">
		<short-description>output format for VCF/BCF: z:gzip vcf v:vcf b:bcf (default v)</short-description>
	</option>
</xsl:template>

<xsl:template match="macro[@name='bam.output']">
	<macro name="compression_level"/>
	<option name="bam_output_format" type="string" short-option="O" long-option="bam-output-format" arg-name="format" default-value='"SAM"'>
		<short-description>output format for sam: SAM, BAM, CRAM</short-description>
	</option>
</xsl:template>


<xsl:template match="macro[@name='version.help']">	
	<macro name="help"/>
	<macro name="version"/>
</xsl:template>



<xsl:template match="*">
<xsl:copy>
<xsl:copy-of select="@*"/>
<xsl:choose>
	<xsl:when test="@type='int' and not(@parser)"><xsl:attribute name="parser">this-&gt;parseInt</xsl:attribute></xsl:when>
	<xsl:when test="@type='char' and not(@parser)"><xsl:attribute name="parser">this-&gt;parseChar</xsl:attribute></xsl:when>
</xsl:choose>
<xsl:apply-templates select="*|text()"/>
</xsl:copy>
</xsl:template>

<xsl:template match="text()">
<xsl:copy>
<xsl:apply-templates select="*|text()"/>
</xsl:copy>
</xsl:template>

</xsl:stylesheet>
