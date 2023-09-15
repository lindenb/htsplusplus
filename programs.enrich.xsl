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


<xsl:template match="macro[@name='reference']">
		<option name="reference" type="string" short-option="R" long-option="reference" arg-name="fasta">
			<xsl:if test="@required='true'">
			<xsl:attribute name="required">true</xsl:attribute>
			</xsl:if>
			<short-description>FASTA reference</short-description>
		</option>
</xsl:template>


<xsl:template match="macro[@name='version.help']">
	<macro name="help"/>
	<macro name="version"/>
</xsl:template>

<xsl:template match="*|text()">
<xsl:copy>
<xsl:copy-of select="@*"/>
<xsl:apply-templates select="*"/>
</xsl:copy>
</xsl:template>

</xsl:stylesheet>
