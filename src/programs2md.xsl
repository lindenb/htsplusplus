<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
	version="1.0"
	>
<xsl:output method="text" indent="yes"/>

<xsl:template match="/">
# hts++


# Compilation

compilation requires a C++ compiler and `xsltproc`.

```
git clone "https://github.com/lindenb/htsplusplus"
cd htsplusplus/src
# set the path to a compiled C htslib directory
make HTSLIB=/path/to/htslib
```


<xsl:apply-templates select="programs"/>

# Author

Pierre Lindenbaum Phd 2022.
</xsl:template>


<xsl:template match="programs">

# subcommands

<xsl:apply-templates select="program[not(@enabled='false')]"/>

</xsl:template>


<xsl:template match="program">

##  <xsl:value-of select="translate(@name,'abcdefghijklmnopqrstuvwxyz','ABCDEFGHIJKLMNOPQRSTUVWXYZ')"/><xsl:text>

</xsl:text>

<xsl:apply-templates select="." mode="short-desc"/>

### Usage

```
hts++ <xsl:value-of select="@name"/> [options...] (files)
```


### Options

| short-option | long-option | argument | description | default-value |
|--------------|-------------|----------|-------------|---------------|
<xsl:for-each select="option[not(@hidden) or not(@hidden='false')]">
<xsl:text>| </xsl:text>
    <xsl:if test="@short-option">
        <xsl:text>-</xsl:text>
        <xsl:value-of select="@short-option"/>
    </xsl:if>
<xsl:text> | </xsl:text>
    <xsl:if test="@long-option">
        <xsl:text>--</xsl:text>
        <xsl:value-of select="@long-option"/>
    </xsl:if>
<xsl:text> | </xsl:text>
    <xsl:choose>
		<xsl:when test="@type = 'bool'"></xsl:when>
		<xsl:when test="@arg-name"><xsl:text>  &lt;</xsl:text><xsl:value-of select="@arg-name"/><xsl:text>&gt;  </xsl:text></xsl:when>
		<xsl:otherwise><xsl:text>  </xsl:text>&lt;ARG&gt;<xsl:text>  </xsl:text></xsl:otherwise>
	</xsl:choose>
<xsl:text> | </xsl:text>
    <xsl:variable name="shorts"><xsl:apply-templates select="." mode="short-desc"/></xsl:variable>
    <xsl:value-of select="normalize-space($shorts)"/>
<xsl:text> | </xsl:text>
    <xsl:apply-templates select="@default"/>
<xsl:text> |
</xsl:text>
</xsl:for-each>

<xsl:apply-templates select="." mode="long-desc"/>


<xsl:text>

</xsl:text>


</xsl:template>



<xsl:template match="br">
<xsl:text>\\n</xsl:text>
</xsl:template>


<xsl:template match="b">
<xsl:text>**</xsl:text>
<xsl:apply-templates/>
<xsl:text>**</xsl:text>
</xsl:template>


<xsl:template match="i">
<xsl:text>_</xsl:text>
<xsl:apply-templates/>
<xsl:text>_</xsl:text>
</xsl:template>

<xsl:template match="code">
<xsl:text>`</xsl:text>
<xsl:apply-templates/>
<xsl:text>`</xsl:text>
</xsl:template>


<xsl:template match="pre">
<xsl:text>\\n```\\n</xsl:text>
<xsl:apply-templates/>
<xsl:text>\\n```\\n</xsl:text>
</xsl:template>

<xsl:template match="quote">
<xsl:text>\"</xsl:text>
<xsl:apply-templates/>
<xsl:text>\"</xsl:text>
</xsl:template>


<xsl:template match="a">
<xsl:value-of select="@href"/>
</xsl:template>

<xsl:template match="text()">
<xsl:value-of select="."/>
</xsl:template>

<xsl:template match="h1">
<xsl:text>
## </xsl:text>
<xsl:apply-templates/>
<xsl:text>
</xsl:text>
</xsl:template>

<xsl:template match="program|programs|option" mode="short-desc">
<xsl:choose>
	<xsl:when test="short-description">
		<xsl:apply-templates select="short-description"/>
	</xsl:when>
	<xsl:when test="description">
		<xsl:apply-templates select="description"/>
	</xsl:when>
	<xsl:when test="long-description">
		<xsl:apply-templates select="short-description"/>
	</xsl:when>
	<xsl:otherwise>
		<xsl:text>no description available</xsl:text>
		<xsl:message terminate="no">description missing for <xsl:value-of select="@name"/></xsl:message>
	</xsl:otherwise>
</xsl:choose>
</xsl:template>


</xsl:stylesheet>
