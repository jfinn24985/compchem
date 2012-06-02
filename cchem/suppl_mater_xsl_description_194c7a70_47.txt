<?xml version="1.0" ?>
<xsl:stylesheet version="1.0"
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<!-- ============================================================= -->
<!--
   Supplementary Material

 This, stylesheet converts a "program.dtd" valid XML file to a
   textual representation suitable for the supplementary material
   needed by a scientific journal.

atylesheet version 1.00 for version 1.00 of the chkcalc package

Date: 18 December 2002
Author:  Justin Finnerty
Current Author: $Author: finnerty $
Date: $Date: 2009/09/04 15:49:49 $

$Log: suppl-mater.xsl,v $
Revision 1.4  2009/09/04 15:49:49  finnerty
Updated the DTD and corresponding XSLT files.

Revision 1.3  2008/06/24 18:00:28  finnerty
Last commit for version 1.0

Revision 1.2  2008/01/15 20:56:46  finnerty
Updated for program.dtd v3

Revision 1.1  2008/01/12 12:37:22  finnerty
Initial version

Revision 1.1  2002/12/18 04:40:31  finnerty
Initial revision


$Revision: 1.4 $

                                                                   -->
<!-- ============================================================= -->

  <xsl:output method="text"/>
  <xsl:strip-space elements="*"/>

  <xsl:include href="rtf-styles.xsl"/>
  <xsl:include href="program-util.xsl"/>
  
  <xsl:variable name="vibration-style" select="vibration-table-standard"/>
  <!-- xsl:variable name="vibration-style" select="vibration-table-compact"/ -->
  <xsl:variable name="config-node" select="document('suppl-parts.xml')/suppl-mater"/>
  <xsl:variable name="style" select="$config-node/base-style"/>
  <!-- xsl:variable name="style" select="compact"/ -->
  <xsl:variable name="energysummary-fstyle" select="energy-summary-freq-standard"/>
  <xsl:variable name="energysummary-ostyle" select="energy-summary-opt-standard"/>
  <xsl:variable name="scale-factor-ext" select="$config-node/scale-value"/>
  <xsl:variable name="scale-factor-ref" select="$config-node/scale-ref"/>

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

  <!-- If this is 7 then include raman activity, if 5 then don't -->
  <xsl:variable name="use-raman" select="$config-node/use-raman"/>

  <xsl:variable name="scale-factor">
  <xsl:choose>
    <xsl:when test="$scale-factor-ext"><xsl:value-of select="$scale-factor-ext"/></xsl:when>
    <xsl:otherwise>1.0</xsl:otherwise>
  </xsl:choose>
  </xsl:variable>

  <xsl:variable name="scale-factor-ref-internal">
  <xsl:choose>
    <xsl:when test="$scale-factor-ref"><xsl:value-of select="$scale-factor-ref"/></xsl:when>
    <xsl:otherwise>(unscaled)</xsl:otherwise>
  </xsl:choose>
  </xsl:variable>

  <xsl:template match="/">
    <!-- Output a heading for the supplementary material -->
    <xsl:call-template name="document-start"/>
    <xsl:call-template name="heading-one">
      <xsl:with-param name="heading-text" select="'Supplementary Material'"/>
    </xsl:call-template>
    <xsl:message terminate="no">
      <xsl:text>Working style "</xsl:text>
      <xsl:value-of select="$style"/>
      <xsl:text>"</xsl:text>
   </xsl:message>

    <!-- 
      Handle each compound in document order using
      compound-formula-memo-visit.

      This means that we will get calls to compound/formula start/end
      and memo(mode=?).
 
      stages:
        - energy-summary (freq, sp)
        - structure (freq)
        - vibration-table (freq)
  -->
    <xsl:message terminate="no">Raman Activity "<xsl:value-of select="$use-raman"/>" (5=no,7=yes)</xsl:message>
    <xsl:call-template name="compound-formula-memo-visit">
      <xsl:with-param name="p1" select="document('suppl-parts.xml')//stage"/>
      <xsl:with-param name="project-name" select="'default'"/>
    </xsl:call-template>
    <xsl:call-template name="document-end"/>
  </xsl:template>

  <!-- Set default behavior to descend nodes but have no output -->
  <xsl:template match="*" mode="compact">
    <xsl:apply-templates mode="compact"/>
  </xsl:template>

  <xsl:template match="*" mode="standard">
    <xsl:apply-templates mode="standard"/>
  </xsl:template>

  <xsl:template match="text()" mode="compact"/>
  <xsl:template match="text()" mode="standard"/>


  <xsl:template match="memo">
    <xsl:param name="p1"/>
    <xsl:param name="p2"/>
    <xsl:param name="p3"/>
    <xsl:choose>
      <xsl:when test="$style='compact'">
        <xsl:apply-templates select="." mode="compact">
          <xsl:with-param name="p1" select="$p1"/>
          <xsl:with-param name="p2" select="$p2"/>
          <xsl:with-param name="p3" select="$p3"/>
        </xsl:apply-templates>
      </xsl:when>
      <xsl:when test="$style='standard'">
        <xsl:apply-templates select="." mode="standard">
          <xsl:with-param name="p1" select="$p1"/>
          <xsl:with-param name="p2" select="$p2"/>
          <xsl:with-param name="p3" select="$p3"/>
        </xsl:apply-templates>
      </xsl:when>
      <xsl:otherwise>
        <xsl:message terminate="yes">
          <xsl:text>Unknown style </xsl:text>
          <xsl:value-of select="$style"/>
        </xsl:message>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="compound-start">
    <xsl:variable name="compound-name">
      <xsl:value-of select="identity[@type='iupac-name']"/>
    </xsl:variable>
    <xsl:variable name="compound-label">
      <xsl:choose>
        <xsl:when test="identity[@type='label']">
          <xsl:value-of select="identity[@type='label']"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="@label"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <xsl:call-template name="heading-two">
      <xsl:with-param name="heading-text">
        <xsl:text>Compound </xsl:text>
        <xsl:value-of select="$compound-name"/>
        <xsl:text> </xsl:text>
        <xsl:call-template name="font-bold">
          <xsl:with-param name="the-text" select="$compound-label"/>
        </xsl:call-template>
      </xsl:with-param>
    </xsl:call-template>
    <xsl:call-template name="paragraph-end"/>
    <!-- xsl:apply-templates select="info"/ -->
  </xsl:template>

  <xsl:template name="compound-end"/>
  <xsl:template name="experiment-start"/>
  <xsl:template name="experiment-end"/>
  <xsl:template name="visit-start"/>
  <xsl:template name="visit-end"/>
 
  <xsl:template name="formula-start">
    <xsl:variable name="compound-label">
      <xsl:choose>
        <xsl:when test="identity[@type='label']">
          <xsl:value-of select="../identity[@type='label']"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="../@label"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <!-- Formula subheading -->
    <xsl:call-template name="heading-three">
      <xsl:with-param name="heading-text">
                <xsl:choose>
                  <xsl:when test="identity[@type='label']">
                    <xsl:value-of select="identity[@type='label']"/>
                  </xsl:when>
                  <xsl:otherwise>
                    <xsl:value-of select="@label"/>
                  </xsl:otherwise>
                </xsl:choose>
      </xsl:with-param>
    </xsl:call-template>
    <xsl:call-template name="paragraph-end"/>
  </xsl:template>

  <xsl:template name="formula-end">
    <!-- Handle each geometry in document order -->
    <xsl:call-template name="paragraph-end"/>
   </xsl:template>

  <xsl:template match="memo" mode="compact">
    <xsl:param name="p1"/>
    <xsl:param name="p2"/>
    <xsl:param name="p3"/>
    <xsl:choose>
      <xsl:when test="$p1='energy-summary'">
        <xsl:apply-templates select="datum[@outcome='route']"/>
        <xsl:if test="datum[@outcome='type' and text()='Freq']|datum[@outcome='type' and text()='SP']|datum[@outcome='type' and text()='FOpt']">
          <xsl:choose>
            <xsl:when test="datum[@outcome='type' and text()='FOpt']">
              <xsl:text>
Geometry optimisation calculation: Program version = </xsl:text>
            </xsl:when>
            <xsl:when test="datum[@outcome='type' and text()='Freq']">
              <xsl:text>
Frequency calculation: Program version = </xsl:text>
            </xsl:when>
            <xsl:when test="datum[@outcome='type' and text()='SP']">
              <xsl:text>
Single-point energy calculation: Program version = </xsl:text>
            </xsl:when>
          </xsl:choose>
          <xsl:value-of select="datum[@outcome='version']"/>
          <xsl:text>. Method and basis set = </xsl:text>
          <xsl:value-of select="datum[@outcome='method']"/>
          <xsl:text>/</xsl:text>
          <xsl:value-of select="datum[@outcome='basis-set']"/>
          <xsl:text>. Calculated Energy </xsl:text>
          <xsl:for-each select="datum[@outcome='hf']|.//datum[@outcome='MP2']">
            <xsl:choose>
              <xsl:when test="@outcome='hf'">
                <xsl:text>E(HF) = </xsl:text>
                <xsl:choose>
                  <xsl:when test="@scale='hartree'">
                    <xsl:value-of select="."/>
                    <xsl:text> (hartree), </xsl:text>
                    <xsl:value-of select=". * $hartree-to-kcalmol * $calmol-to-jmol"/>
                    <xsl:text> (kJ mol{{\*\updnprop5801}\up8\fs12 -1}). </xsl:text>
                  </xsl:when>
                  <xsl:when test="@scale='kjmol'">
                    <xsl:value-of select="."/>
                    <xsl:text> (kJ mol{{\*\updnprop5801}\up8\fs12 -1}). </xsl:text>
                  </xsl:when>
                  <xsl:otherwise>
                    <xsl:message terminate="no">(Error retrieving energy from database). </xsl:message>
                  </xsl:otherwise>
                </xsl:choose>
              </xsl:when>
              <xsl:otherwise>
                <xsl:text>E(</xsl:text>
                <xsl:value-of select="@outcome"/>
                <xsl:text>) = </xsl:text>
                <!-- TODO assumes energy is hartree -->
                <xsl:value-of select="."/>
                <xsl:text> (hartree), </xsl:text>
                <xsl:value-of select=". * $hartree-to-kcalmol * $calmol-to-jmol"/>
                <xsl:text> (kJ mol{{\*\updnprop5801}\up8\fs12 -1}). </xsl:text>
               </xsl:otherwise>
            </xsl:choose>
          </xsl:for-each>
        </xsl:if>
      </xsl:when>
      <xsl:when test="$p1='structure'">
	<xsl:choose>
          <xsl:when test="datum[@outcome='type' and text()='Freq']">
            <xsl:apply-templates select="datum[@outcome='atom-list']/datum[@outcome='structure']" mode="compact"/>
          </xsl:when>
          <xsl:when test="datum[@outcome='type' and text()='FOpt']">
            <xsl:apply-templates select="datum[@outcome='atom-list']/datum[@outcome='structure']" mode="compact"/>
          </xsl:when>
	</xsl:choose>
      </xsl:when>
      <xsl:when test="$p1='vibration-table'">
        <xsl:apply-templates select="datum[@outcome='vibration-table']" mode="compact"/>
      </xsl:when>
    </xsl:choose>
  </xsl:template>

  <xsl:template match="memo" mode="standard">
    <xsl:param name="p1"/>
    <xsl:param name="p2"/>
    <xsl:param name="p3"/>
    <xsl:choose>
      <xsl:when test="$p1='energy-summary'">
        <xsl:if test="datum[@outcome='type' and text()='Freq']|datum[@outcome='type' and text()='SP']">
          <xsl:call-template name="table-start">
            <xsl:with-param name="columns" select="1"/>
          </xsl:call-template>
          <xsl:call-template name="table-row-start">
            <xsl:with-param name="columns" select="1"/>
          </xsl:call-template>
          <xsl:apply-templates select="datum[@outcome='route']"/>
          <xsl:choose>
            <xsl:when test="datum[@outcome='type' and text()='Freq']">
              <xsl:text>
Frequency calculation</xsl:text>
            </xsl:when>
            <xsl:when test="datum[@outcome='type' and text()='SP']">
              <xsl:text>
Single-point energy calculation</xsl:text>
            </xsl:when>
          </xsl:choose>
          <xsl:call-template name="table-row-end"/>
          <xsl:call-template name="table-row-start">
            <xsl:with-param name="columns" select="1"/>
          </xsl:call-template>
          <xsl:text>Program version: </xsl:text>
          <xsl:value-of select="datum[@outcome='version']"/>
          <xsl:text>. </xsl:text>
          <xsl:call-template name="table-row-end"/>
          <xsl:call-template name="table-row-start">
            <xsl:with-param name="columns" select="1"/>
          </xsl:call-template>
          <xsl:text>Theory level: </xsl:text>
          <xsl:value-of select="datum[@outcome='method']"/>
          <xsl:text>/</xsl:text>
          <xsl:value-of select="datum[@outcome='basis-set']"/>
          <xsl:text>. </xsl:text>
          <xsl:call-template name="table-row-end"/>
          <xsl:call-template name="table-row-start">
            <xsl:with-param name="columns" select="1"/>
          </xsl:call-template>
          <xsl:text>Calculated Energy: </xsl:text>
          <xsl:for-each select="datum[@outcome='hf']|.//datum[@outcome='MP2']">
            <xsl:choose>
              <xsl:when test="@outcome='hf'">
                <xsl:text>E(HF) = </xsl:text>
                <xsl:choose>
                  <xsl:when test="@scale='hartree'">
                    <xsl:value-of select="."/>
                    <xsl:text> (hartree), </xsl:text>
                    <xsl:value-of select=". * $hartree-to-kcalmol * $calmol-to-jmol"/>
                    <xsl:text> (kJ mol{{\*\updnprop5801}\up8\fs12 -1}). </xsl:text>
                  </xsl:when>
                  <xsl:when test="@scale='kjmol'">
                    <xsl:value-of select="."/>
                    <xsl:text> (kJ mol{{\*\updnprop5801}\up8\fs12 -1}). </xsl:text>
                  </xsl:when>
                  <xsl:otherwise>
                    <xsl:message terminate="no">(Error retrieving energy from database). </xsl:message>
                  </xsl:otherwise>
                </xsl:choose>
              </xsl:when>
              <xsl:otherwise>
                <xsl:text>E(</xsl:text>
                <xsl:value-of select="@outcome"/>
                <xsl:text>) = </xsl:text>
                <!-- TODO assumes energy is hartree -->
                <xsl:value-of select="."/>
                <xsl:text> (hartree), </xsl:text>
                <xsl:value-of select=". * $hartree-to-kcalmol * $calmol-to-jmol"/>
                <xsl:text> (kJ mol{{\*\updnprop5801}\up8\fs12 -1}). </xsl:text>
               </xsl:otherwise>
            </xsl:choose>
          </xsl:for-each>
          <xsl:call-template name="table-row-end"/>
          <xsl:call-template name="table-end"/>
          <xsl:call-template name="paragraph-end"/>
        </xsl:if>
      </xsl:when>
      <xsl:when test="$p1='structure'">
        <xsl:if test="datum[@outcome='type' and text()='Freq']">
          <xsl:apply-templates select="datum[@outcome='atom-list']/datum[@outcome='structure']" mode="standard"/>
        </xsl:if>
      </xsl:when>
      <xsl:when test="$p1='vibration-table'">
        <xsl:apply-templates select="datum[@outcome='vibration-table']" mode="standard"/>
      </xsl:when>
    </xsl:choose>
  </xsl:template>

  <xsl:template match="datum[@outcome='vibration-table']" mode="compact">
    <xsl:text>
Harmonic Frequencies (Number, Frequency (cm{{\*\updnprop5801}\up8\fs12 -1}), IR Intensity (km mol{{\*\updnprop5801}\up10\fs12 -1})</xsl:text>
    <xsl:if test="$use-raman=7">
      <xsl:text>, Raman Activity</xsl:text>
    </xsl:if>
    <xsl:text>): </xsl:text>
   <xsl:apply-templates mode="compact" select="datum[@outcome='vibration']"/>
   <xsl:call-template name="paragraph-end"/>
  </xsl:template>

  <xsl:template match="datum[@outcome='vibration']" mode="compact">
    <xsl:if test="number(@ordinal)!=0">
      <xsl:text>{\i </xsl:text>
      <xsl:value-of select="@ordinal"/>
      <xsl:text>}: </xsl:text>
      <xsl:value-of select="format-number(datum[@outcome='frequency'] * $scale-factor, '#0.##')"/>
      <xsl:text>, </xsl:text>
      <xsl:value-of select="format-number(datum[@outcome='IR-intensity'], '#0.##')"/>
      <xsl:if test="$use-raman=7">
        <xsl:text>, </xsl:text>
        <xsl:value-of select="format-number(datum[@outcome='raman-activity'], '#0.##')"/>
      </xsl:if>
      <xsl:text>; </xsl:text>
    </xsl:if>
  </xsl:template>

  <xsl:template match="datum[@outcome='vibration-table']" mode="standard">
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
        <xsl:value-of select="$use-raman"/>
      </xsl:with-param>
    </xsl:call-template>
    <xsl:call-template name="table-row-start">
      <xsl:with-param name="columns">
        <xsl:value-of select="$use-raman"/>
      </xsl:with-param>
    </xsl:call-template>
    <xsl:text>Mode</xsl:text>
    <xsl:call-template name="table-next-datum"/>
    <xsl:text>Frequency{\up8\fs12\i a}</xsl:text>
    <xsl:call-template name="table-next-datum"/>
    <xsl:text>Reduced Mass</xsl:text>
    <xsl:call-template name="table-next-datum"/>
    <xsl:text>Force Constants</xsl:text>
    <xsl:call-template name="table-next-datum"/>
    <xsl:text>IR Intensity</xsl:text>
    <xsl:if test="$use-raman=7">
      <xsl:call-template name="table-next-datum"/>
      <xsl:text>Raman Activity</xsl:text>
      <xsl:call-template name="table-next-datum"/>
      <xsl:text>Depolar</xsl:text>
    </xsl:if>
    <xsl:call-template name="table-row-end"/>
    <!-- table-start, table-end, table-row-start, table-row-end, table-next-datum -->
    <xsl:apply-templates mode="standard" select="datum[@outcome='vibration']"/>
    <xsl:call-template name="table-row-start">
      <xsl:with-param name="columns">
        <xsl:value-of select="1"/>
      </xsl:with-param>
    </xsl:call-template>
    <xsl:text>{\up8\fs12\i a} Frequencies have been scaled by </xsl:text>
    <xsl:value-of select="$scale-factor"/>
    <xsl:text> </xsl:text>
    <xsl:value-of select="$scale-factor-ref-internal"/>

    <xsl:call-template name="table-row-end"/>
    <xsl:call-template name="table-end"/>
  </xsl:template>

  <xsl:template match="datum[@outcome='vibration']" mode="standard">
    <xsl:call-template name="table-row-start">
      <xsl:with-param name="columns">
        <xsl:value-of select="$use-raman"/>
      </xsl:with-param>
    </xsl:call-template>
    <xsl:value-of select="datum[@outcome='mode']"/>
    <xsl:call-template name="table-next-datum"/>
    <xsl:value-of select="format-number( datum[@outcome='frequency'] * $scale-factor, '#0.####' )"/>
    <xsl:call-template name="table-next-datum"/>
    <xsl:value-of select="datum[@outcome='reduced-mass']"/>
    <xsl:call-template name="table-next-datum"/>
    <xsl:value-of select="datum[@outcome='force-constants']"/>
    <xsl:call-template name="table-next-datum"/>
    <xsl:value-of select="datum[@outcome='IR-intensity']"/>
    <xsl:if test="$use-raman=7">
      <xsl:call-template name="table-next-datum"/>
      <xsl:value-of select="datum[@outcome='raman-activity']"/>
      <xsl:call-template name="table-next-datum"/>
      <xsl:value-of select="datum[@outcome='depolar']"/>
    </xsl:if>
    <xsl:call-template name="table-row-end"/>
  </xsl:template>

  <xsl:template match="datum[@outcome='structure']" mode="standard">
    <xsl:choose>
      <xsl:when test="@format = 'axyz'">
        <xsl:call-template name="make-table">
          <xsl:with-param name="list-string">
            <xsl:text>Structure definition (3D Coordinates)</xsl:text>
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
        <xsl:call-template name="make-table">
          <xsl:with-param name="list-string">
            <xsl:text>Atom,X coord/ Angstrom,Y coord/ Angstrom,Z coord/ Angstrom</xsl:text>
          </xsl:with-param>
          <xsl:with-param name="table-width">
            <xsl:value-of select="4"/>
          </xsl:with-param>
          <xsl:with-param name="column-divider">
            <xsl:value-of select="','"/>
          </xsl:with-param>
          <xsl:with-param name="row-divider">
            <xsl:value-of select="'&#92;'"/>
          </xsl:with-param>
        </xsl:call-template>
        <xsl:call-template name="make-table">
          <xsl:with-param name="list-string">
            <xsl:value-of select="."/>
          </xsl:with-param>
          <xsl:with-param name="table-width">
            <xsl:value-of select="4"/>
          </xsl:with-param>
          <xsl:with-param name="column-divider">
            <xsl:value-of select="','"/>
          </xsl:with-param>
          <xsl:with-param name="row-divider">
            <xsl:value-of select="'&#92;'"/>
          </xsl:with-param>
        </xsl:call-template>
        <xsl:call-template name="paragraph-end"/>
      </xsl:when>     
      <xsl:when test="@format = 'alad'">
        <xsl:call-template name="make-table">
          <xsl:with-param name="list-string">
            <xsl:text>Structure definition (Z-matrix)</xsl:text>
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
        <xsl:call-template name="make-table">
          <xsl:with-param name="list-string">
            <xsl:text>Atom, Centre 1, Length/ &#197;, Centre 2, Angle/ &#176;, Centre 3, Dihedral/ &#176;</xsl:text>
          </xsl:with-param>
          <xsl:with-param name="table-width">
            <xsl:value-of select="8"/>
          </xsl:with-param>
          <xsl:with-param name="column-divider">
            <xsl:value-of select="','"/>
          </xsl:with-param>
          <xsl:with-param name="row-divider">
            <xsl:value-of select="'&#92;'"/>
          </xsl:with-param>
        </xsl:call-template>
        <xsl:call-template name="make-table">
          <xsl:with-param name="list-string">
            <xsl:value-of select="substring-before(.,'&#92;&#92;')"/>
          </xsl:with-param>
          <xsl:with-param name="table-width">
            <xsl:value-of select="8"/>
          </xsl:with-param>
          <xsl:with-param name="column-divider">
            <xsl:value-of select="','"/>
          </xsl:with-param>
          <xsl:with-param name="row-divider">
            <xsl:value-of select="'&#92;'"/>
          </xsl:with-param>
        </xsl:call-template>
        <xsl:call-template name="make-table">
          <xsl:with-param name="list-string">
            <xsl:value-of select="''"/>
          </xsl:with-param>
          <xsl:with-param name="table-width">
            <xsl:value-of select="8"/>
          </xsl:with-param>
          <xsl:with-param name="column-divider">
            <xsl:value-of select="','"/>
          </xsl:with-param>
          <xsl:with-param name="row-divider">
            <xsl:value-of select="'&#92;'"/>
          </xsl:with-param>
        </xsl:call-template>
        <xsl:call-template name="make-table">
          <xsl:with-param name="list-string">
            <xsl:value-of select="substring-after(.,'&#92;&#92;')"/>
          </xsl:with-param>
          <xsl:with-param name="table-width">
            <xsl:value-of select="8"/>
          </xsl:with-param>
          <xsl:with-param name="column-divider">
            <xsl:value-of select="'='"/>
          </xsl:with-param>
          <xsl:with-param name="row-divider">
            <xsl:value-of select="'&#92;'"/>
          </xsl:with-param>
        </xsl:call-template>
        <xsl:call-template name="paragraph-end"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:text>Stylesheet cannot handle format "</xsl:text>
        <xsl:value-of select="@format"/>
        <xsl:text>" atom-lists, content:</xsl:text>
        <xsl:call-template name="paragraph-end"/>
        <xsl:text>[[</xsl:text>
        <xsl:value-of select="."/>
        <xsl:text>]]</xsl:text>
        <xsl:call-template name="paragraph-end"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template match="datum[@outcome='structure']" mode="compact">
    <xsl:choose>
      <xsl:when test="@format = 'axyz'">
        <xsl:text>Structure definition (3D Coordinates, Atom, X coord/ Angstrom, Y coord/ Angstrom, Z coord/ Angstrom): </xsl:text>
        <xsl:call-template name="replaceAllSubStrings">
          <xsl:with-param name="stringToSearchIn">
            <xsl:call-template name="replaceAllSubStrings">
              <xsl:with-param name="stringToSearchIn">
                <xsl:value-of select="."/>
              </xsl:with-param>
              <xsl:with-param name="subString">
                <xsl:value-of select="'\'"/>
              </xsl:with-param>
              <xsl:with-param name="replaceString">
                <xsl:value-of select="'; '"/>
              </xsl:with-param>
            </xsl:call-template>
          </xsl:with-param>
          <xsl:with-param name="subString">
            <xsl:value-of select="','"/>
          </xsl:with-param>
          <xsl:with-param name="replaceString">
            <xsl:value-of select="', '"/>
          </xsl:with-param>
        </xsl:call-template>
        <xsl:text>.</xsl:text>
        <xsl:call-template name="paragraph-end"/>
      </xsl:when>     
      <xsl:when test="@format = 'alad'">
        <xsl:text>Structure definition (Z-matrix, Atom, Centre 1, Length/ &#197;, Centre 2, Angle/ &#176;, Centre 3, Dihedral/ &#176;): </xsl:text>
        <xsl:call-template name="replaceAllSubStrings">
          <xsl:with-param name="stringToSearchIn">
            <xsl:call-template name="replaceAllSubStrings">
              <xsl:with-param name="stringToSearchIn">
                <xsl:value-of select="."/>
              </xsl:with-param>
              <xsl:with-param name="subString">
                <xsl:value-of select="'\'"/>
              </xsl:with-param>
              <xsl:with-param name="replaceString">
                <xsl:value-of select="'; '"/>
              </xsl:with-param>
            </xsl:call-template>
          </xsl:with-param>
          <xsl:with-param name="subString">
            <xsl:value-of select="','"/>
          </xsl:with-param>
          <xsl:with-param name="replaceString">
            <xsl:value-of select="', '"/>
          </xsl:with-param>
        </xsl:call-template>
        <xsl:text>.</xsl:text>
        <xsl:call-template name="paragraph-end"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:text>Stylesheet cannot handle format "</xsl:text>
        <xsl:value-of select="@format"/>
        <xsl:text>", atom-lists content:</xsl:text>
        <xsl:call-template name="paragraph-end"/>
        <xsl:call-template name="replaceAllSubStrings">
          <xsl:with-param name="stringToSearchIn">
            <xsl:call-template name="replaceAllSubStrings">
              <xsl:with-param name="stringToSearchIn">
                <xsl:value-of select="."/>
              </xsl:with-param>
              <xsl:with-param name="subString">
                <xsl:value-of select="'\'"/>
              </xsl:with-param>
              <xsl:with-param name="replaceString">
                <xsl:value-of select="'; '"/>
              </xsl:with-param>
            </xsl:call-template>
          </xsl:with-param>
          <xsl:with-param name="subString">
            <xsl:value-of select="','"/>
          </xsl:with-param>
          <xsl:with-param name="replaceString">
            <xsl:value-of select="', '"/>
          </xsl:with-param>
        </xsl:call-template>
        <xsl:text>.</xsl:text>
        <xsl:call-template name="paragraph-end"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template match="datum[@outcome='route']">
    <xsl:variable name="route" select="text()"/>
    <xsl:if test="contains($route, 'SCRF')">
      <xsl:text>Solvent field (PCM model) calculation </xsl:text>
      <xsl:choose>
        <xsl:when test="contains($route, 'ETHER')">
          <xsl:text>with ether. </xsl:text>
        </xsl:when>
        <xsl:when test="contains($route, 'ACETONE')">
          <xsl:text>with acetone. </xsl:text>
        </xsl:when>
        <xsl:when test="contains($route, 'WATER')">
          <xsl:text>with water. </xsl:text>
        </xsl:when>
        <xsl:when test="contains($route, 'ACETONITRILE')">
          <xsl:text>with acetonitrile. </xsl:text>
        </xsl:when>
      </xsl:choose>
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
