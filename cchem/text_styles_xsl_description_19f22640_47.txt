<?xml version="1.0" ?>
<xsl:stylesheet version="1.0"
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<!-- ============================================================= -->
<!--
   TExt-only/CSV styles

   This stylesheet is one of a set of stylesheets for textual
   output. It provides a set of target-dependent templates for
   formatting output, in this case ***ASCII text + comma-separated
   value***. Careful use of this set of templates should allow client
   stylesheets to be independent of output format.

Stylesheet version 1.00 for version 1.00 of the chkcalc package

Date: 18 December 2002
Author:  Justin Finnerty
Current Author: $Author: finnerty $
Date: $Date: 2008/01/12 12:37:22 $

$Log: text-styles.xsl,v $
Revision 1.1  2008/01/12 12:37:22  finnerty
Initial version

Revision 1.1  2002/12/18 04:41:19  finnerty
Initial revision


$Revision: 1.1 $

                                                                   -->
<!-- ============================================================= -->

  <xsl:output method="text"/>

  <!-- This style files provides some basic templates that can be -->
  <!-- used to format documents for TEXT and CSV output. -->

  <!-- Author: Justin Finnerty -->
  <!-- Date: December 2002 -->

  <!-- This style file provides the following templates -->
  <!-- document-start, document-end -->
  <!-- table-start, table-end, table-row-start, table-row-end, table-next-datum -->
  <!-- heading-one, heading-two, heading-three -->
  <!-- font-bold, font-slanted -->


  <!-- In this style a document start/end has no special meaning -->
  <xsl:template name="document-start">
  </xsl:template>

  <xsl:template name="document-end">
  </xsl:template>

  <xsl:template name="paragraph-end">
    <xsl:text>

</xsl:text>
  </xsl:template>

  <!-- In this style a table is represented as comma-separated-values -->
  <xsl:template name="table-start">
    <xsl:param name="columns"/><!-- unused here -->
  </xsl:template>

  <xsl:template name="table-end">
  </xsl:template>

  <xsl:template name="table-row-start">
    <xsl:param name="columns"/><!-- unused here -->
    <xsl:text>"</xsl:text>
  </xsl:template>

  <xsl:template name="table-row-end">
    <xsl:text>"
</xsl:text>
  </xsl:template>

  <xsl:template name="table-next-datum">
    <xsl:text>","</xsl:text>
  </xsl:template>

  <!-- In this style a heading is represented by multiple asterices -->
  <xsl:template name="heading-one">
    <xsl:param name="heading-text"/>
    <xsl:text>* </xsl:text>
    <xsl:value-of select="$heading-text"/>
    <xsl:text>

</xsl:text>
  </xsl:template>

  <xsl:template name="heading-two">
    <xsl:param name="heading-text"/>
    <xsl:text>** </xsl:text>
    <xsl:value-of select="$heading-text"/>
    <xsl:text>

</xsl:text>
  </xsl:template>

  <xsl:template name="heading-three">
    <xsl:param name="heading-text"/>
    <xsl:text>*** </xsl:text>
    <xsl:value-of select="$heading-text"/>
    <xsl:text>

</xsl:text>
  </xsl:template>

  <!-- In this style bold and slanted/italic are psuedo represented -->
  <xsl:template name="font-bold">
    <xsl:param name="the-text"/>
    <xsl:text>*</xsl:text>
    <xsl:value-of select="$the-text"/>
    <xsl:text>*</xsl:text>
  </xsl:template>

  <xsl:template name="font-slanted">
    <xsl:param name="the-text"/>
    <xsl:text>_</xsl:text>
    <xsl:value-of select="$the-text"/>
    <xsl:text>_</xsl:text>
  </xsl:template>

</xsl:stylesheet>
