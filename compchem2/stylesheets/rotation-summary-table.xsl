<?xml version="1.0" ?>
<xsl:stylesheet version="1.0"
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<!-- ============================================================= -->
<!--
   Supplementary Material

   This stylesheet converts a "program.dtd" valid XML file to a
   textual representation suitable for the supplementary material
   needed by a scientific journal.

Stylesheet version 1.00 for version 1.00 of the chkcalc package

Date: 18 December 2002
Author:  Justin Finnerty
Current Author: $Author: finnerty $
Date: $Date: 2008/01/12 12:37:22 $

$Log: rotation-summary-table.xsl,v $
Revision 1.1  2008/01/12 12:37:22  finnerty
Initial version

Revision 1.1  2002/12/18 04:40:31  finnerty
Initial revision


$Revision: 1.1 $

                                                                   -->
<!-- ============================================================= -->

  <xsl:output method="text"/>
  <xsl:strip-space elements="*"/>

  <xsl:include href="rtf-styles.xsl"/>
  <xsl:include href="program-util.xsl"/>

  <xsl:variable name="scale-factor">
    0.9613
  </xsl:variable>

  <xsl:template match="/">
    <!-- Output a heading for the supplementary material -->
    <xsl:call-template name="document-start"/>

    <!-- Handle each compound in document order -->
    <xsl:apply-templates/>

    <xsl:call-template name="document-end"/>
  </xsl:template>

  <xsl:template match="program">
    <xsl:call-template name="make-table">
      <xsl:with-param name="list-string">
        <xsl:text>Harmonic Frequencies</xsl:text>
      </xsl:with-param>
      <xsl:with-param name="table-width">
        <xsl:value-of select="1"/>
      </xsl:with-param>
      <xsl:with-param name="column-divider">
        <xsl:value-of select="','"/>
      </xsl:with-param>
      <xsl:with-param name="row-divider">
        <xsl:value-of select="'&#92;'"/>
      </xsl:with-param>
    </xsl:call-template>
    <xsl:call-template name="table-start">
      <xsl:with-param name="columns">
        <xsl:value-of select="4"/>
      </xsl:with-param>
    </xsl:call-template>
    <xsl:call-template name="table-row-start">
      <xsl:with-param name="columns">
        <xsl:value-of select="4"/>
      </xsl:with-param>
    </xsl:call-template>
    <xsl:text>Structure</xsl:text>
    <xsl:call-template name="table-next-datum"/>
    <xsl:text>stretch(C2,C3,X){\i a}</xsl:text>
    <xsl:call-template name="table-next-datum"/>
    <xsl:text>stretch(C1,X1){\i a}</xsl:text>
    <xsl:call-template name="table-next-datum"/>
    <xsl:text>NImag</xsl:text>
    <xsl:call-template name="table-next-datum"/>
    <xsl:call-template name="table-row-end"/>
    <xsl:call-template name="table-end"/>    
    <!-- table-start, table-end, table-row-start, table-row-end, table-next-datum -->
    <xsl:apply-templates select="//compound|//comp-src"/>
    <xsl:call-template name="table-start">
      <xsl:with-param name="columns">
        <xsl:value-of select="1"/>
      </xsl:with-param>
    </xsl:call-template>
    <xsl:call-template name="table-row-start">
      <xsl:with-param name="columns">
        <xsl:value-of select="1"/>
      </xsl:with-param>
    </xsl:call-template>
    <xsl:text>{\i a} Frequencies have been scaled by</xsl:text>
    <xsl:value-of select="$scale-factor"/>
    <xsl:text>Ref. Wong, M. W. {\i Chem. Phys. Lett.} {\b 1996}, {\i 256}, 391-399.</xsl:text>

    <xsl:call-template name="table-row-end"/>
    <xsl:call-template name="table-end"/>    
  </xsl:template>

  <xsl:template match="compound">
    <xsl:call-template name="table-start">
      <xsl:with-param name="columns">
        <xsl:value-of select="1"/>
      </xsl:with-param>
    </xsl:call-template>
    <xsl:call-template name="table-row-start">
      <xsl:with-param name="columns">
        <xsl:value-of select="1"/>
      </xsl:with-param>
    </xsl:call-template>
    <xsl:text>{\b </xsl:text>
    <xsl:value-of select="@label"/>
    <xsl:text>}</xsl:text>
    <xsl:call-template name="table-row-end"/>
    <xsl:call-template name="table-end"/>    
    <xsl:call-template name="table-start">
      <xsl:with-param name="columns">
        <xsl:value-of select="4"/>
      </xsl:with-param>
    </xsl:call-template>
    <xsl:apply-templates select="geometry|geom-src"/>
    <xsl:call-template name="table-end"/>    
  </xsl:template>

  <xsl:template match="geometry">
    <xsl:call-template name="table-row-start">
      <xsl:with-param name="columns">
        <xsl:value-of select="4"/>
      </xsl:with-param>
    </xsl:call-template>
     <xsl:text>{\i </xsl:text>
    <xsl:value-of select="@label"/>
    <xsl:text>}</xsl:text>
    <xsl:call-template name="table-next-datum"/>   
    <xsl:apply-templates select="experiment"/>
    <xsl:call-template name="table-row-end"/>
  </xsl:template>

  <xsl:template match="experiment">
    <xsl:if test="calc-method[@type='Freq']">
      <xsl:apply-templates select=".//dan[@name='vibration']" mode="vibration-summary">
        <xsl:with-param name="low-freq" select="1920"/>
        <xsl:with-param name="hi-freq" select="2300"/>
      </xsl:apply-templates>
      <xsl:call-template name="table-next-datum"/>
      <xsl:apply-templates select=".//dan[@name='vibration']" mode="vibration-summary">
        <xsl:with-param name="low-freq" select="1660"/>
        <xsl:with-param name="hi-freq" select="1885"/>
      </xsl:apply-templates>
      <xsl:call-template name="table-next-datum"/>
      <xsl:value-of select="dan[@name='NImag']"/>
    </xsl:if>
  </xsl:template>

  <xsl:template match="dan" mode="vibration-summary">
    <xsl:param name="low-freq"/>
    <xsl:param name="hi-freq"/>
    <xsl:if test="dan[@name='frequency'] &gt; $low-freq and dan[@name='frequency'] &lt; $hi-freq">
      <xsl:value-of select="ceiling((dan[@name='frequency'] * $scale-factor) - 0.5 )"/>
      <xsl:text>(</xsl:text>
      <xsl:value-of select="ceiling(dan[@name='IR-intensity'] - 0.5 )"/>
      <xsl:text>)</xsl:text>
    </xsl:if>
  </xsl:template>

  <!-- table-start, table-end, table-row-start, table-row-end, table-next-datum -->

  <xsl:template name="make-table">
    <xsl:param name="list-string"/>
    <xsl:param name="table-width"/>
    <xsl:param name="column-divider"/>
    <xsl:param name="row-divider"/>

    <!-- Start the table -->
    <xsl:call-template name="table-start">
      <xsl:with-param name="columns">
        <xsl:value-of select="$table-width"/>
      </xsl:with-param>
    </xsl:call-template>

    <!-- Version 1 only splits by line -->
    <xsl:call-template name="make-table-by-row">
      <xsl:with-param name="list-string">
        <xsl:value-of select="$list-string"/>
      </xsl:with-param>
      <xsl:with-param name="table-width">
        <xsl:value-of select="$table-width"/>
      </xsl:with-param>
      <xsl:with-param name="column-divider">
        <xsl:value-of select="$column-divider"/>
      </xsl:with-param>
      <xsl:with-param name="row-divider">
        <xsl:value-of select="$row-divider"/>
      </xsl:with-param>
    </xsl:call-template>

    <!-- Finish the table -->
    <xsl:call-template name="table-end"/>
  </xsl:template>
  
  <xsl:template name="make-table-by-row">
    <xsl:param name="list-string"/>
    <xsl:param name="table-width"/>
    <xsl:param name="column-divider"/>
    <xsl:param name="row-divider"/>

    <!-- Version 1 only splits by line -->
    <xsl:choose>
      <xsl:when test="contains($list-string,$row-divider)">
        <!-- Start the row -->
        <xsl:call-template name="table-row-start">
          <xsl:with-param name="columns" select="$table-width"/>
        </xsl:call-template>
        <!-- Output the row -->
        <!-- <xsl:text>Row text [</xsl:text> -->
        <xsl:call-template name="make-table-by-row-column">
          <xsl:with-param  name="row-string">
            <xsl:value-of select="substring-before($list-string,$row-divider)"/>
          </xsl:with-param>
          <xsl:with-param name="column-divider">
            <xsl:value-of select="$column-divider"/>
          </xsl:with-param>
        </xsl:call-template>
        <!-- <xsl:text>]</xsl:text> -->
        <!-- End the row -->
        <xsl:call-template name="table-row-end"/>
        <!-- Do recursive call -->
        <xsl:call-template  name="make-table-by-row">
          <xsl:with-param name="list-string">
            <xsl:value-of select="substring-after($list-string, $row-divider)"/>
          </xsl:with-param>
          <xsl:with-param name="table-width">
            <xsl:value-of select="$table-width"/>
          </xsl:with-param>
          <xsl:with-param name="column-divider">
            <xsl:value-of select="$column-divider"/>
          </xsl:with-param>
          <xsl:with-param name="row-divider">
            <xsl:value-of select="$row-divider"/>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <!-- Start the row -->
        <xsl:call-template name="table-row-start">
          <xsl:with-param name="columns" select="$table-width"/>
        </xsl:call-template>
        <!-- End recursion stage, output whats left -->
        <!-- <xsl:text>Last Row text [</xsl:text> -->
        <xsl:call-template name="make-table-by-row-column">
          <xsl:with-param  name="row-string">
            <xsl:value-of select="$list-string"/>
          </xsl:with-param>
          <xsl:with-param name="column-divider">
            <xsl:value-of select="$column-divider"/>
          </xsl:with-param>
        </xsl:call-template>        
        <!-- <xsl:text>]</xsl:text> -->
        <!-- End the row -->
        <xsl:call-template name="table-row-end"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="make-table-by-row-column">
    <xsl:param name="row-string"/>
    <xsl:param name="column-divider"/>
    <xsl:choose>
      <xsl:when test="contains($row-string, $column-divider)">
        <!-- in recursive section -->
        <!-- get column entry -->
        <!-- <xsl:text>Row text [</xsl:text> -->
        <xsl:value-of select="substring-before($row-string, $column-divider)"/>
        <!-- <xsl:text>]</xsl:text> -->
        <!-- get column divider -->
        <xsl:call-template name="table-next-datum"/>
        <!-- Do recursive call -->
        <xsl:call-template  name="make-table-by-row-column">
          <xsl:with-param name="row-string">
            <xsl:value-of select="substring-after($row-string, $column-divider)"/>
          </xsl:with-param>
          <xsl:with-param name="column-divider">
            <xsl:value-of select="$column-divider"/>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <!-- in recursive section -->
        <!-- get column entry -->
        <!-- <xsl:text>Last Row text [</xsl:text> -->
        <xsl:value-of select="$row-string"/>
        <!-- <xsl:text>]</xsl:text> -->
        <!-- no column divider -->
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

</xsl:stylesheet>
