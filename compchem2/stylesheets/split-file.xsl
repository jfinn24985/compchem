<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0"
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:redirect="org.apache.xalan.xslt.extensions.Redirect"
  extension-element-prefixes="redirect">
<!-- ============================================================= -->
<!--
  File Splitter

  This stylesheet splits the input file into a set of
  sub-documents. This follows the multi-level nature of the DTD and
  allows a large data base to be broken into smaller file. This also
  converts from the dtd version 1.0 to the dtd version 2.0

  Date: 06 August 2007
  Date: $Date: 2009/09/04 15:49:49 $
  
  $Log: split-file.xsl,v $
  Revision 1.5  2009/09/04 15:49:49  finnerty
  Updated the DTD and corresponding XSLT files.

  Revision 1.4  2008/06/24 18:00:28  finnerty
  Last commit for version 1.0

  Revision 1.3  2008/01/15 20:56:46  finnerty
  Updated for program.dtd v3

  Revision 1.2  2008/01/13 17:55:48  finnerty
  Updated for program.dtd v3

  Revision 1.2  2008/01/12 12:37:41  finnerty
  Initial version
  
  Revision 1.1  2008/01/12 12:37:22  finnerty
  Initial version
  
  $Revision: 1.5 $
  
                                                                   -->
<!-- ============================================================= -->

  <xsl:output method="xml" doctype-public="http://www.openscience.net.au/datascheme/V2.0.0/" doctype-system="/home/finnerty/Office/Projects/compchem/schema/program.dtd" version="1.0" encoding="UTF-8"/>

  <xsl:variable name="baseDir">
    <!-- <xsl:text>~/.chkcalc/db/</xsl:text> -->
  </xsl:variable>

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

  <!--======================================-->
  <!-- translate 'apparatus' to 'mechanism' -->
  <xsl:template match="apparatus">
    <xsl:element name="mechanism">
      <xsl:if test="@label">
	<xsl:attribute name="label"><xsl:value-of select="@label"/></xsl:attribute>
      </xsl:if>
      <xsl:if test=".">
	<!-- need to wrap 'apparatus' content in 'info' element -->
	<xsl:element name="info">
	  <xsl:attribute name="name">information</xsl:attribute>
	  <xsl:apply-templates/>
	</xsl:element>
      </xsl:if>
    </xsl:element>
  </xsl:template>

  <!--======================================-->
  <!-- translate 'atom-list' to nix -->
  <xsl:template match="atom-list">
    <!-- The atom-list is no longer used.  Now it
         is a 'datum' set within an 'experiment'. 
         The 'experiment' template attempts to do
         the translation. -->
  </xsl:template>

  <!--======================================-->
  <!-- translate 'atom-list' to "datum" -->
  <xsl:template match="atom-list" mode="in-experiment">
    <xsl:element name="datum">
      <xsl:attribute name="outcome"><xsl:text>atom-list</xsl:text></xsl:attribute>
      <xsl:if test="@charge">
        <xsl:element name="datum">
          <xsl:attribute name="outcome"><xsl:text>charge</xsl:text></xsl:attribute>
          <xsl:value-of select="@charge"/>
        </xsl:element>
      </xsl:if>
      <xsl:if test="@multiplicity">
        <xsl:element name="datum">
          <xsl:attribute name="outcome"><xsl:text>multiplicity</xsl:text></xsl:attribute>
          <xsl:value-of select="@multiplicity"/>
        </xsl:element>
      </xsl:if>
      <xsl:element name="datum">
        <xsl:attribute name="outcome"><xsl:text>structure</xsl:text></xsl:attribute>
        <xsl:attribute name="format"><xsl:value-of select="@format"/></xsl:attribute>
        <xsl:if test="."><xsl:apply-templates/></xsl:if>
      </xsl:element>
    </xsl:element>
  </xsl:template>

  <!--======================================-->
  <!-- translate 'calc-program' to 'datum' -->
  <!-- This should be processed within a 
       'mech-memo' element in the new doc
       which will use the @name attr. -->
  <xsl:template match="calc-program">
    <xsl:element name="datum">
      <xsl:attribute name="outcome"><xsl:text>version</xsl:text></xsl:attribute>
      <xsl:if test="."><xsl:apply-templates/></xsl:if>
    </xsl:element>
  </xsl:template>

  <!--======================================-->
  <!-- translate 'calc-program' to 'datum' -->
  <!-- This should be processed within a 
       'mech-memo' element in the new doc. -->
  <xsl:template match="calc-method">
    <xsl:if test="@method">
      <xsl:element name="datum">
	<xsl:attribute name="outcome"><xsl:text>method</xsl:text></xsl:attribute>
	<xsl:value-of select="@method"/>
      </xsl:element>
    </xsl:if>
    <xsl:if test="@basis-set">
      <xsl:element name="datum">
	<xsl:attribute name="outcome"><xsl:text>basis-set</xsl:text></xsl:attribute>
	<xsl:value-of select="@basis-set"/>
      </xsl:element>
    </xsl:if>
    <xsl:if test="@type">
      <xsl:element name="datum">
	<xsl:attribute name="outcome"><xsl:text>type</xsl:text></xsl:attribute>
	<xsl:value-of select="@type"/>
      </xsl:element>
    </xsl:if>
    <xsl:if test=".">
      <xsl:element name="datum">
	<xsl:attribute name="outcome"><xsl:text>route</xsl:text></xsl:attribute>
	<xsl:apply-templates/>
      </xsl:element>
    </xsl:if>
  </xsl:template>

  <!--======================================-->
  <!-- translate 'chem-notation' to nix -->
  <xsl:template match="chem-notation">
    <!-- The atom-list is no longer used.  Now it
         is a 'datum' set within an 'experiment'. 
         The 'experiment' template attempts to do
         the translation. -->
  </xsl:template>

  <!--====================================-->
  <!-- translate 'compound' to 'compound' v2 -->
  <xsl:template match="compound" mode="top">
    <!-- Write cross-reference element to current doc -->
    <xsl:element name="{name()}">
	<xsl:for-each select="@*">
	  <xsl:choose>
	    <xsl:when test="'chem-note' = name()">
	      <xsl:attribute name="category"><xsl:value-of select="."/></xsl:attribute>
	    </xsl:when>
	    <xsl:otherwise>
	      <xsl:attribute name="{name()}"><xsl:value-of select="."/></xsl:attribute>
	    </xsl:otherwise>
	  </xsl:choose>
	</xsl:for-each>
	<xsl:if test=".">
        <xsl:apply-templates select="iupac-name"/>
        <xsl:apply-templates select="information"/>
        <xsl:element name="identity">
          <xsl:attribute name="type"><xsl:text>label</xsl:text></xsl:attribute>
          <xsl:value-of select="@label"/>
        </xsl:element>
        <xsl:apply-templates select="geometry" mode="top"/>
      </xsl:if>
    </xsl:element>
    <xsl:text>
</xsl:text>
  </xsl:template>

   <!--====================================-->
  <!-- translate 'compound' to 'comp-src' -->
  <xsl:template match="compound" mode="proj">
    <xsl:apply-templates select="geometry" mode="proj"/>
  </xsl:template>

  <!--======================================-->
  <!-- translate 'comp-program' to 'mechanism' -->
  <xsl:template match="comp-program">
    <xsl:element name="mechanism">
      <xsl:if test="@name">
	<xsl:attribute name="label"><xsl:value-of select="@name"/></xsl:attribute>
      </xsl:if>
      <xsl:if test=".">
	<!-- need to wrap 'comp-program' content in 'text' element -->
	<xsl:element name="info">
	  <xsl:attribute name="name">information</xsl:attribute>
	  <xsl:apply-templates/>
	</xsl:element>
      </xsl:if>
    </xsl:element>
  </xsl:template>

  <!--======================================-->
  <!-- translate 'dan' to 'datum' -->
  <xsl:template match="dan">
    <xsl:element name="datum">
      <xsl:for-each select="@*">
	<xsl:choose>
	  <xsl:when test="'unit' = name()">
	    <xsl:attribute name="scale"><xsl:value-of select="."/></xsl:attribute>
	  </xsl:when>
	  <xsl:when test="'name' = name()">
	    <xsl:attribute name="outcome"><xsl:value-of select="."/></xsl:attribute>
	  </xsl:when>
	  <xsl:otherwise>
	    <xsl:attribute name="{name()}"><xsl:value-of select="."/></xsl:attribute>
	  </xsl:otherwise>
	</xsl:choose>
      </xsl:for-each>
      <xsl:apply-templates/>
    </xsl:element>
  </xsl:template>

  <!--=====================================================-->
  <!-- extract 'experiment' from 'geometry' into 'project' -->
  <xsl:template match="geometry" mode="proj">
    <xsl:variable name="pageref" select="experiment/@pageref"/><!-- Use first pageref -->
    <xsl:element name="experiment">
      <xsl:attribute name="reference"><xsl:value-of select="$pageref"/></xsl:attribute>
      <xsl:for-each select="@*">
	<xsl:choose>
	  <xsl:when test="'pageref' = name()"/><!-- Do nothing -->
	  <xsl:when test="'type'=name()"/><!-- Do nothing -->
	  <xsl:when test="'label'=name()"/><!-- Do nothing -->
	  <xsl:otherwise>
	    <xsl:attribute name="{name()}"><xsl:value-of select="."/></xsl:attribute>
	  </xsl:otherwise>	  
	</xsl:choose>
      </xsl:for-each>
      <xsl:if test="not(@date-start)">
	<xsl:attribute name="date-start">unknown</xsl:attribute>
      </xsl:if>
      <xsl:if test="not(@worker)">
	<xsl:attribute name="worker">finnerty</xsl:attribute>
      </xsl:if>
      <xsl:apply-templates select="information"/>
      <xsl:for-each select="experiment">
        <!--======================================-->
        <!-- translate 'experiment' to 'experiment' -->
        <xsl:variable name="filename">
          <xsl:value-of select="concat (../../@label,'/', @pageref, '.xml')"/>
        </xsl:variable>
        <!-- Write cross-reference element to current doc -->
        <xsl:element name="memo-src">
          <xsl:attribute name="label"><xsl:value-of select="@pageref"/></xsl:attribute>
          <xsl:attribute name="formula"><xsl:value-of select="../@label"/></xsl:attribute>
          <xsl:attribute name="compound"><xsl:value-of select="../../@label"/></xsl:attribute>
          <xsl:attribute name="href">
            <xsl:value-of select="$filename"/>
          </xsl:attribute>
        </xsl:element>
        <!-- Write compound and children to new doc -->
        <redirect:write select="concat($baseDir, $filename)" doctype-public="http://www.openscience.net.au/datascheme/V2.0.0/" doctype-system="/home/finnerty/Office/Projects/compchem/schema/program.dtd" version="1.0" encoding="UTF-8">
          <xsl:element name="memo">
            <xsl:attribute name="label"><xsl:value-of select="@pageref"/></xsl:attribute>
            <xsl:attribute name="formula"><xsl:value-of select="../@label"/></xsl:attribute>
            <xsl:attribute name="compound"><xsl:value-of select="../../@label"/></xsl:attribute>
            <xsl:if test="calc-program">
              <xsl:attribute name="mechanism"><xsl:value-of select="calc-program/@name"/></xsl:attribute>
              <xsl:apply-templates select="calc-program"/>
            </xsl:if>
            <xsl:if test="calc-method">
              <xsl:apply-templates select="calc-method"/>
            </xsl:if>
            <xsl:for-each select="../atom-list[@pageref=$pageref]">
              <xsl:if test="position()=1">
                <xsl:apply-templates select="." mode="in-experiment"/>
              </xsl:if>
            </xsl:for-each>
            <xsl:apply-templates select="dan"/>
          </xsl:element>
        </redirect:write>
      </xsl:for-each>
    </xsl:element>
    <xsl:text>
</xsl:text>
  </xsl:template>

  <!--===================================-->
  <!-- translate 'geometry' to 'formula' -->
  <xsl:template match="geometry" mode="top">
    <xsl:element name="formula">
      <xsl:attribute name="label"><xsl:value-of select="@label"/></xsl:attribute>
      <xsl:apply-templates select="information"/>
      <xsl:element name="identity">
        <xsl:attribute name="type"><xsl:text>label</xsl:text></xsl:attribute>
        <xsl:value-of select="@label"/>
      </xsl:element>
    </xsl:element>      
    <xsl:text>
</xsl:text>
  </xsl:template>

  <!--===========================-->
  <!-- translate 'information' to 'info' -->
  <xsl:template match="information">
    <xsl:element name="info">
      <xsl:attribute name="name">
        <xsl:text>information</xsl:text>
      </xsl:attribute>
      <xsl:if test=".">
        <xsl:apply-templates/>
      </xsl:if>
    </xsl:element>
  </xsl:template>

  <!--===========================-->
  <!-- translate 'iupac-name' to 'identity' -->
  <xsl:template match="iupac-name">
    <xsl:element name="identity">
      <xsl:attribute name="type">iupac-name</xsl:attribute>
      <xsl:apply-templates/>
    </xsl:element>
  </xsl:template>

  <!--===========================-->
  <!-- translate 'program' to 'program' -->
  <xsl:template match="program">
    <xsl:element name="program">
      <xsl:apply-templates select="information"/>
      <xsl:apply-templates select="worker"/>
      <xsl:apply-templates select="apparatus"/>
      <xsl:apply-templates select="comp-program"/>
      <xsl:apply-templates select="compound" mode="top"/>
      <xsl:element name="project">
	<xsl:attribute name="label"><xsl:text>default</xsl:text></xsl:attribute>
	<xsl:apply-templates select="compound" mode="proj"/>
      </xsl:element>
    </xsl:element>
  </xsl:template>

  <!--===========================-->
  <!-- translate 'tan' to 'info' -->
  <xsl:template match="tan">
    <xsl:element name="info">
      <xsl:choose>
	<xsl:when test="@type">
          <xsl:attribute name="name">
            <xsl:value-of select="@type"/>
          </xsl:attribute>
        </xsl:when>
	<xsl:otherwise>
          <xsl:attribute name="name">information</xsl:attribute>
        </xsl:otherwise>
      </xsl:choose>
      <xsl:if test=".">
        <xsl:apply-templates/>
      </xsl:if>
    </xsl:element>
  </xsl:template>

  <!--===========================-->
  <!-- translate 'worker' to 'worker' -->
  <xsl:template match="worker">
    <xsl:element name="worker"><xsl:for-each select="@*"><xsl:attribute name="{name()}"><xsl:value-of select="."/></xsl:attribute></xsl:for-each>
      <xsl:element name="info"><xsl:attribute name="name">information</xsl:attribute><xsl:apply-templates select="text()"/></xsl:element>
      
      <xsl:apply-templates select="tan"/>
    </xsl:element>
  </xsl:template>

  <!-- Catch all template -->
  <xsl:template match="*">
    <xsl:element name="{name()}"><xsl:for-each select="@*"><xsl:attribute name="{name()}"><xsl:value-of select="."/></xsl:attribute></xsl:for-each><xsl:if test="."><xsl:apply-templates/></xsl:if></xsl:element>
  </xsl:template>
</xsl:stylesheet>
