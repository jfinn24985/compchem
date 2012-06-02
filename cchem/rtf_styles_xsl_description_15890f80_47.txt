<?xml version="1.0" ?>
<xsl:stylesheet version="1.0"
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<!-- ============================================================= -->
<!--
   RTF styles

   This stylesheet is one of a set of stylesheets for textual
   output. It provides a set of target-dependent templates for
   formatting output, in this case ***Rich-Text Format***. Careful use
   of this set of templates should allow client stylesheets to be
   independent of output format.

Stylesheet version 1.00 for version 1.00 of the chkcalc package

Date: 18 December 2002
Author:  Justin Finnerty
Current Author: $Author: finnerty $
Date: $Date: 2008/01/15 20:56:46 $

$Log: rtf-styles.xsl,v $
Revision 1.2  2008/01/15 20:56:46  finnerty
Updated for program.dtd v3

Revision 1.1  2008/01/12 12:37:22  finnerty
Initial version

Revision 1.1  2002/12/18 04:40:59  finnerty
Initial revision


$Revision: 1.2 $

                                                                   -->
<!-- ============================================================= -->

  <xsl:output method="text"/>

  <!-- This style files provides some basic templates that can be -->
  <!-- used to format documents for RTF output. -->

  <!-- Author: Justin Finnerty -->
  <!-- Date: December 2002 -->

  <!-- This style file provides the following templates -->
  <!-- document-start, document-end -->
  <!-- paragraph-end -->
  <!-- table-start, table-end, table-row-start, table-row-end, table-next-datum -->
  <!-- heading-one, heading-two, heading-three -->
  <!-- font-bold, font-slanted -->


  <!-- In this style a document start/end has no special meaning -->
  <xsl:template name="document-start">
    <xsl:text>{\rtf0\ansi
{\fonttbl
{\f0\fswiss Helvetica;}
{\f1\froman Times;}}
\paperw12240\paperh15840\margl1800\margr1800\margt1440\margb1440
\ftnbj\aenddoc\ftnrstcont\aftnrstcont\ftnnar\aftnnrlc
\sectd\pgwsxn11908\pghsxn16847\marglsxn720\margtsxn720\margrsxn720
\margbsxn720
{\stylesheet{\s0\f1\fs20\lang1031\snext0 Normal;}
{\s1\sb240\sa60\keepn{\*\pn \pnlvl1\pndec\pnprev1\pnstart1\pnsp144 {\pntxta .}}\b\f0\fs32\lang2057\kerning28 \sbasedon0\snext0 Heading 1;}
{\s2\sb240\sa60\keepn{\*\pn \pnlvl2\pndec\pnprev1\pnstart1\pnsp144 }\b\f0\fs24\lang2057 \sbasedon0\snext0 Heading 2;}
{\s3\sb240\sa60\keepn{\*\pn \pnlvl3\pndec\pnprev1\pnstart1\pnsp144 {\pntxtb .}}\b\f0\fs24\lang2057 \sbasedon0\snext0 Heading 3;}
{\s4\sb240\sa60\keepn{\*\pn \pnlvl4\pndec\pnprev1\pnstart1\pnsp144 {\pntxtb .}}\b\f0\fs24\lang2057 \sbasedon0\snext0 Heading 4;}
{\*\cs10 \additive Default Paragraph Font;}}
{\*\pnseclvl1\pnucrm\pnstart1\pnindent720\pnhang{\pntxta .}}
{\*\pnseclvl2\pnucltr\pnstart1\pnindent720\pnhang{\pntxta .}}
{\*\pnseclvl3\pndec\pnstart1\pnindent720\pnhang{\pntxta .}}
{\*\pnseclvl4\pnlcltr\pnstart1\pnindent720\pnhang{\pntxta .}}
{\footer\pard\fi-1\li1\ri1\qr\plain
\f1\fs20{S\field{\*\fldinst{PAGE }}{\fldrslt{?}}}}
\f1\fs20
</xsl:text>
  </xsl:template>

  <xsl:template name="document-end">
    <xsl:text>
}</xsl:text>
  </xsl:template>

  <xsl:template name="paragraph-end">
    <xsl:text> \par </xsl:text>
  </xsl:template>

  <!-- In this style a table is quite complex, with cell widths being calculated -->
  <xsl:template name="table-start">
    <xsl:param name="columns"/>
    <xsl:text>{</xsl:text>
  </xsl:template>

  <xsl:template name="table-end">
    <xsl:text>}</xsl:text>
  </xsl:template>

  <xsl:template name="table-row-start">
    <xsl:param name="columns"/>
    <xsl:text>\trowd \trqc \trgaph88\trrh402 </xsl:text>
    <xsl:call-template name="define-columns">
      <xsl:with-param name="total-columns" select="$columns"/>
      <xsl:with-param name="columns-so-far" select="1"/>
    </xsl:call-template>
    <xsl:text> {</xsl:text>
  </xsl:template>

  <xsl:template name="table-row-end">
    <xsl:text>}\cell \pard \intbl \row
</xsl:text>
  </xsl:template>

  <xsl:template name="table-next-datum">
    <xsl:text>} \cell \pard \intbl {</xsl:text>
  </xsl:template>

  <!-- In this style a heading is defined using styles, defined in document start section -->
  <xsl:template name="heading-one">
    <xsl:param name="heading-text"/>
    <xsl:text>{\s1 </xsl:text>
    <xsl:value-of select="$heading-text"/>
    <xsl:text>}\par</xsl:text>
  </xsl:template>

  <xsl:template name="heading-two">
    <xsl:param name="heading-text"/>
    <xsl:text>{\s2 </xsl:text>
    <xsl:value-of select="$heading-text"/>
    <xsl:text>}\par</xsl:text>
  </xsl:template>

  <xsl:template name="heading-three">
    <xsl:param name="heading-text"/>
    <xsl:text>{\s3 </xsl:text>
    <xsl:value-of select="$heading-text"/>
    <xsl:text>}\par</xsl:text>
  </xsl:template>

  <!-- In this style bold and slanted/italic are natively represented -->
  <xsl:template name="font-bold">
    <xsl:param name="the-text"/>
    <xsl:text>{\b </xsl:text>
    <xsl:value-of select="$the-text"/>
    <xsl:text>}</xsl:text>
  </xsl:template>

  <xsl:template name="font-slanted">
    <xsl:param name="the-text"/>
    <xsl:text>{\i </xsl:text>
    <xsl:value-of select="$the-text"/>
    <xsl:text>}</xsl:text>
  </xsl:template>

  <!-- RTF specific templates -->
  <xsl:variable name="page-width" select="10000.0"/>

  <xsl:template name="define-columns">
    <!-- This generates the \cellx??? widths for the columns -->
    <xsl:param name="total-columns"/>
    <xsl:param name="columns-so-far"/>
    <xsl:text>\cellx</xsl:text>
    <xsl:value-of select="round( ( $page-width div $total-columns ) * $columns-so-far )"/>
    <xsl:if test="$total-columns &gt; $columns-so-far">
      <xsl:call-template name="define-columns">
        <xsl:with-param name="total-columns" select="$total-columns"/>
        <xsl:with-param name="columns-so-far" select="$columns-so-far + 1"/>
      </xsl:call-template>
    </xsl:if>
  </xsl:template>

</xsl:stylesheet>
