<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0"
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:redirect="org.apache.xalan.xslt.extensions.Redirect"
  extension-element-prefixes="redirect">
<!-- ============================================================= -->
<!--
   File UN-Splitter

     This stylesheet unsplits the split input files that result from
     the split-file.xsl stylesheet. This allows a large data base to
     be broken into smaller file edited then rebuild.

Stylesheet version 1.00 for version 1.00 of the chkcalc package

Date: 02 August 2007
Author:  Justin Finnerty
Current Author: $Author: finnerty $
Date: $Date: 2008/01/15 20:56:46 $

$Log: unsplit-file.xsl,v $
Revision 1.2  2008/01/15 20:56:46  finnerty
Updated for program.dtd v3

Revision 1.1  2008/01/12 12:37:22  finnerty
Initial version

$Revision: 1.2 $

                                                                   -->
<!-- ============================================================= -->

  <xsl:output method="xml" doctype-public="http://www.openscience.net.au/datascheme/V2.0.0/" doctype-system="/home/finnerty/Office/Projects/compchem/schema/program.dtd" version="1.0" encoding="UTF-8"/>

  <!-- Get program-util to collapse memo-src and proj-src links -->
  <xsl:include href="program-util.xsl"/>

  <xsl:template match="/">
    <!-- Put out DTD info -->
    <xsl:apply-templates/>
  </xsl:template>

  <xsl:template match="processing-instruction()">
    <xsl:processing-instruction name="{name()}"><xsl:value-of select="."/></xsl:processing-instruction>
  </xsl:template>

  <xsl:template match="comment()">
    <xsl:comment><xsl:value-of select="."/></xsl:comment>
  </xsl:template>

  <!-- Catch all template -->
  <xsl:template match="*">
    <xsl:element name="{name()}"><xsl:for-each select="@*"><xsl:attribute name="{name()}"><xsl:value-of select="."/></xsl:attribute></xsl:for-each><xsl:if test="."><xsl:apply-templates/></xsl:if></xsl:element>
  </xsl:template>
</xsl:stylesheet>
