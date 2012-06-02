<?xml version="1.0" ?>
<xsl:stylesheet version="1.0"
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<!-- ============================================================= -->
<!--
   Energy

   This stylesheet converts a "program.dtd" valid XML file to a
   table of thermochemistry information.

Stylesheet version 1.00 for version 1.00 of the chkcalc package

Date: 18 December 2002
Author:  Justin Finnerty
Current Author: $Author: finnerty $
Date: $Date: 2009/09/04 15:49:49 $

$Log: energy.xsl,v $
Revision 1.5  2009/09/04 15:49:49  finnerty
Updated the DTD and corresponding XSLT files.

Revision 1.4  2008/06/24 18:00:27  finnerty
Last commit for version 1.0

Revision 1.3  2008/01/13 18:38:09  finnerty
Updated for program.dtd v3

Revision 1.2  2008/01/13 17:54:16  finnerty
Updated for program.dtd v3

Revision 1.2  2008/01/12 12:37:41  finnerty
Initial version

Revision 1.1  2008/01/12 12:37:22  finnerty
Initial version

Revision 1.1  2002/12/18 04:40:13  finnerty
Initial revision


$Revision: 1.5 $

                                                                   -->
<!-- ============================================================= -->
  <xsl:output method="text"/>

  <xsl:include href="program-util.xsl"/>
  <xsl:include href="text-styles.xsl"/>

  <!-- Constants for converting hartrees to Joules -->
  <xsl:variable name="hartree-to-kcalmol">
    627.5095
  </xsl:variable>

  <xsl:variable name="calmol-to-jmol">
    4.184
  </xsl:variable>

  <!-- Assumption: Temperature is 298.15K -->
  <xsl:variable name="temperature">
    298.15
  </xsl:variable>

  <xsl:variable name="energy-table-width">
    24
  </xsl:variable>

  <xsl:template match="/">
    <!-- Output a header for the output table -->
    <xsl:call-template name="document-start"/>
    <xsl:call-template name="table-start">
      <xsl:with-param name="columns">
        <xsl:value-of select="$energy-table-width"/>
      </xsl:with-param>
    </xsl:call-template>
    <xsl:call-template name="table-row-start">
      <xsl:with-param name="columns">
        <xsl:value-of select="$energy-table-width"/>
      </xsl:with-param>
    </xsl:call-template>
    <xsl:text>Compound</xsl:text>
    <xsl:call-template name="table-next-datum"/>
    <xsl:text>Geometry</xsl:text>               
    <xsl:call-template name="table-next-datum"/>
    <xsl:text>Program</xsl:text>       
    <xsl:call-template name="table-next-datum"/>
    <xsl:text>Method</xsl:text>        
    <xsl:call-template name="table-next-datum"/>
    <xsl:text>Basis</xsl:text>         
    <xsl:call-template name="table-next-datum"/>
    <xsl:text>PageRef</xsl:text>       
    <xsl:call-template name="table-next-datum"/>
    <xsl:text>Type</xsl:text>       
    <xsl:call-template name="table-next-datum"/>
    <xsl:text>E type</xsl:text>       
    <xsl:call-template name="table-next-datum"/>
    <xsl:text>E</xsl:text>       
    <xsl:call-template name="table-next-datum"/>
    <xsl:text>dH(corr)</xsl:text>       
    <xsl:call-template name="table-next-datum"/>
    <xsl:text>ZPVE</xsl:text>       
    <xsl:call-template name="table-next-datum"/>
    <xsl:text>Entropy</xsl:text>       
    <xsl:call-template name="table-next-datum"/>
    <xsl:text>Dipole</xsl:text>       
    <xsl:call-template name="table-next-datum"/>
    <xsl:text>Program</xsl:text>       
    <xsl:call-template name="table-next-datum"/>
    <xsl:text>Method</xsl:text>       
    <xsl:call-template name="table-next-datum"/>
    <xsl:text>Basis</xsl:text>       
    <xsl:call-template name="table-next-datum"/>
    <xsl:text>PageRef</xsl:text>       
    <xsl:call-template name="table-next-datum"/>
    <xsl:text>Type</xsl:text>       
    <xsl:call-template name="table-next-datum"/>
    <xsl:text>E type</xsl:text>       
    <xsl:call-template name="table-next-datum"/>
    <xsl:text>E</xsl:text>       
    <xsl:call-template name="table-next-datum"/>
    <xsl:text>dH(corr)</xsl:text>       
    <xsl:call-template name="table-next-datum"/>
    <xsl:text>ZPVE</xsl:text>       
    <xsl:call-template name="table-next-datum"/>
    <xsl:text>Entropy</xsl:text>       
    <xsl:call-template name="table-next-datum"/>
    <xsl:text>Dipole</xsl:text>    
    <xsl:call-template name="table-row-end"/>

    <!-- Handle each compound in document order -->

    <xsl:call-template name="compound-formula-memo-visit">
      <xsl:with-param name="p1" select="document('energy-parts.xml')//b"/>
      <xsl:with-param name="project-name" select="'default'"/>
    </xsl:call-template>

    <xsl:call-template name="table-end"/>
    <xsl:call-template name="document-end"/>
  </xsl:template>

  <xsl:template name="compound-start"/>
  <xsl:template name="compound-end"/>
  <xsl:template name="experiment-start"/>
  <xsl:template name="experiment-end"/>
  <xsl:template name="visit-start"/>
  <xsl:template name="visit-end"/>
 
  <xsl:template name="formula-end">
    <!-- Handle each geometry in document order -->
    <xsl:call-template name="table-row-end"/>
   </xsl:template>

   <xsl:template name="formula-start">
    <!-- Output the compound label and geometry -->
    <xsl:call-template name="table-row-start">
      <xsl:with-param name="columns">
        <xsl:value-of select="$energy-table-width"/>
      </xsl:with-param>
    </xsl:call-template>
    <xsl:value-of select="../@label"/>
    <xsl:call-template name="table-next-datum"/>
    <xsl:value-of select="@label"/>
    <xsl:call-template name="table-next-datum"/>
  </xsl:template>

  <xsl:template match="memo">
    <xsl:param name="p1"/>
    <xsl:param name="p2"/>
    <xsl:param name="p3"/>
    <!-- Handle the 'Freq' experiments to extract energy data -->
    <!-- <xsl:message terminate="no">
      <xsl:text>IN ENERGY "memo" template p1 = '</xsl:text>
      <xsl:value-of select="$p1"/>
      <xsl:text>' reference = '</xsl:text>
      <xsl:value-of select="$p2"/>
      <xsl:text>'
      </xsl:text>
    </xsl:message> -->
    <xsl:choose>
      <xsl:when test="$p1='thermochemistry'">
        <xsl:if test=".//datum[@outcome='type' and text()='Freq']">
          <!-- If we have a frequency calculation output that -->
          <xsl:apply-templates select="datum[@outcome='version']"/>
          <xsl:call-template name="table-next-datum"/>
          <xsl:apply-templates select="datum[@outcome='method']"/>
          <xsl:call-template name="table-next-datum"/>
          <xsl:apply-templates select="datum[@outcome='basis-set']"/>
          <xsl:call-template name="table-next-datum"/>
          <xsl:value-of select="@label"/>
          <xsl:call-template name="table-next-datum"/>
          <xsl:value-of select=".//datum[@outcome='type']/text()"/>
          <xsl:call-template name="table-next-datum"/>
          <xsl:choose>
            <xsl:when test=".//datum[@outcome='CCSD(T)']">
              <xsl:value-of select="'CCSD(T)'"/>
              <xsl:call-template name="table-next-datum"/>
              <xsl:value-of select=".//datum[@outcome='CCSD(T)']/text()"/>
            </xsl:when>
            <xsl:when test=".//datum[@outcome='QCISD(T)']">
              <xsl:value-of select="'QCISD(T)'"/>
              <xsl:call-template name="table-next-datum"/>
              <xsl:value-of select=".//datum[@outcome='QCISD(T)']/text()"/>
            </xsl:when>
            <xsl:when test=".//datum[@outcome='MP4SDTQ']">
              <xsl:value-of select="'MP4SDTQ'"/>
              <xsl:call-template name="table-next-datum"/>
              <xsl:value-of select=".//datum[@outcome='MP4SDTQ']/text()"/>
            </xsl:when>
            <xsl:when test=".//datum[@outcome='MP4SDTQ']">
              <xsl:value-of select="'MP4SDTQ'"/>
              <xsl:call-template name="table-next-datum"/>
              <xsl:value-of select=".//datum[@outcome='MP4SDTQ']/text()"/>
            </xsl:when>
            <xsl:when test=".//datum[@outcome='MP2']">
              <xsl:value-of select="'MP2'"/>
              <xsl:call-template name="table-next-datum"/>
              <xsl:value-of select=".//datum[@outcome='MP2']/text()"/>
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="'hf'"/>
              <xsl:call-template name="table-next-datum"/>
              <xsl:value-of select=".//datum[@outcome='hf']/text()"/>
            </xsl:otherwise>
          </xsl:choose>
          <xsl:call-template name="table-next-datum"/>
           <xsl:value-of select=".//datum[@outcome='enthalpy-corr']/text()"/>
          <xsl:call-template name="table-next-datum"/>
          <xsl:value-of select=".//datum[@outcome='zero-point-corr']/text()"/>
          <xsl:call-template name="table-next-datum"/>
          <xsl:value-of select=".//datum[@outcome='entropy']/text()"/>
          <xsl:call-template name="table-next-datum"/>
          <xsl:call-template name="last-in-comma-list">
            <xsl:with-param name="tailstring" select=".//datum[@outcome='dipole']/text()"/>
          </xsl:call-template>
          <xsl:call-template name="table-next-datum"/>
        </xsl:if>
      </xsl:when>
      <xsl:when test="$p1='singlepoint'">
        <!-- otherwise add enough blanks -->
        <xsl:if test=".//datum[@outcome='type' and text()='SP']">
          <!-- Handle the experiments to extract energy data -->
          <xsl:apply-templates select="datum[@outcome='version']"/>
          <xsl:call-template name="table-next-datum"/>
          <xsl:apply-templates select="datum[@outcome='method']"/>
          <xsl:call-template name="table-next-datum"/>
          <xsl:apply-templates select="datum[@outcome='basis-set']"/>
          <xsl:call-template name="table-next-datum"/>
          <xsl:value-of select="@label"/>
          <xsl:call-template name="table-next-datum"/>
          <xsl:value-of select=".//datum[@outcome='type']/text()"/>
          <xsl:call-template name="table-next-datum"/>
          <xsl:choose>
            <xsl:when test=".//datum[@outcome='CCSD(T)']">
              <xsl:value-of select="'CCSD(T)'"/>
              <xsl:call-template name="table-next-datum"/>
              <xsl:value-of select=".//datum[@outcome='CCSD(T)']/text()"/>
            </xsl:when>
            <xsl:when test=".//datum[@outcome='QCISD(T)']">
              <xsl:value-of select="'QCISD(T)'"/>
              <xsl:call-template name="table-next-datum"/>
              <xsl:value-of select=".//datum[@outcome='QCISD(T)']/text()"/>
            </xsl:when>
            <xsl:when test=".//datum[@outcome='MP4SDTQ']">
              <xsl:value-of select="'MP4SDTQ'"/>
              <xsl:call-template name="table-next-datum"/>
              <xsl:value-of select=".//datum[@outcome='MP4SDTQ']/text()"/>
            </xsl:when>
            <xsl:when test=".//datum[@outcome='MP4SDTQ']">
              <xsl:value-of select="'MP4SDTQ'"/>
              <xsl:call-template name="table-next-datum"/>
              <xsl:value-of select=".//datum[@outcome='MP4SDTQ']/text()"/>
            </xsl:when>
            <xsl:when test=".//datum[@outcome='MP2']">
              <xsl:value-of select="'MP2'"/>
              <xsl:call-template name="table-next-datum"/>
              <xsl:value-of select=".//datum[@outcome='MP2']/text()"/>
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="'hf'"/>
              <xsl:call-template name="table-next-datum"/>
              <xsl:value-of select=".//datum[@outcome='hf']/text()"/>
            </xsl:otherwise>
          </xsl:choose>
          <xsl:call-template name="table-next-datum"/>
          <xsl:call-template name="table-next-datum"/>
          <xsl:call-template name="table-next-datum"/>
          <xsl:call-template name="table-next-datum"/>
          <xsl:call-template name="last-in-comma-list">
            <xsl:with-param name="tailstring" select=".//datum[@outcome='dipole']/text()"/>
          </xsl:call-template>
          <xsl:call-template name="table-next-datum"/>
        </xsl:if>
      </xsl:when>
      <xsl:otherwise>
        <!-- otherwise add enough blanks -->
        <xsl:if test=".//datum[@outcome='type' and text()='FOpt']">
          <!-- Handle the experiments to extract energy data -->
          <xsl:apply-templates select="datum[@outcome='version']"/>
          <xsl:call-template name="table-next-datum"/>
          <xsl:apply-templates select="datum[@outcome='method']"/>
          <xsl:call-template name="table-next-datum"/>
          <xsl:apply-templates select="datum[@outcome='basis-set']"/>
          <xsl:call-template name="table-next-datum"/>
          <xsl:value-of select="@label"/>
          <xsl:call-template name="table-next-datum"/>
          <xsl:value-of select=".//datum[@outcome='type']/text()"/>
          <xsl:call-template name="table-next-datum"/>
          <xsl:choose>
            <xsl:when test=".//datum[@outcome='CCSD(T)']">
              <xsl:value-of select="'CCSD(T)'"/>
              <xsl:call-template name="table-next-datum"/>
              <xsl:value-of select=".//datum[@outcome='CCSD(T)']/text()"/>
            </xsl:when>
            <xsl:when test=".//datum[@outcome='QCISD(T)']">
              <xsl:value-of select="'QCISD(T)'"/>
              <xsl:call-template name="table-next-datum"/>
              <xsl:value-of select=".//datum[@outcome='QCISD(T)']/text()"/>
            </xsl:when>
            <xsl:when test=".//datum[@outcome='MP4SDTQ']">
              <xsl:value-of select="'MP4SDTQ'"/>
              <xsl:call-template name="table-next-datum"/>
              <xsl:value-of select=".//datum[@outcome='MP4SDTQ']/text()"/>
            </xsl:when>
            <xsl:when test=".//datum[@outcome='MP4SDTQ']">
              <xsl:value-of select="'MP4SDTQ'"/>
              <xsl:call-template name="table-next-datum"/>
              <xsl:value-of select=".//datum[@outcome='MP4SDTQ']/text()"/>
            </xsl:when>
            <xsl:when test=".//datum[@outcome='MP2']">
              <xsl:value-of select="'MP2'"/>
              <xsl:call-template name="table-next-datum"/>
              <xsl:value-of select=".//datum[@outcome='MP2']/text()"/>
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="'hf'"/>
              <xsl:call-template name="table-next-datum"/>
              <xsl:value-of select=".//datum[@outcome='hf']/text()"/>
            </xsl:otherwise>
          </xsl:choose>
          <xsl:call-template name="table-next-datum"/>
          <xsl:call-template name="table-next-datum"/>
          <xsl:call-template name="table-next-datum"/>
          <xsl:call-template name="table-next-datum"/>
          <xsl:call-template name="last-in-comma-list">
            <xsl:with-param name="tailstring" select=".//datum[@outcome='dipole']/text()"/>
          </xsl:call-template>
          <xsl:call-template name="table-next-datum"/>
        </xsl:if>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="last-in-comma-list">
    <xsl:param name="tailstring"/>
    <xsl:choose>
      <xsl:when test="contains($tailstring,',')">
        <xsl:call-template name="last-in-comma-list">
          <xsl:with-param name="tailstring" select="substring-after($tailstring,',')"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$tailstring"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

</xsl:stylesheet>
