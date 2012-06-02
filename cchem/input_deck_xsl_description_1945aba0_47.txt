
<?xml version="1.0" ?>
<xsl:stylesheet version="1.0"
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<!-- ============================================================= -->
<!--
   Input-Deck

   This stylesheet converts a "experiment.dtd" valid XML file to a
   computer program input file.

    stylesheet version 1.00 for version 1.00 of the chkcalc package
        Date: 18 December 2002
Author:  Justin Finnerty
Current Author: $Author: finnerty $
Date: $Date: 2008/01/12 12:37:22 $

$Log: input-deck.xsl,v $
Revision 1.1  2008/01/12 12:37:22  finnerty
Initial version


$Revision: 1.1 $

                                                                   -->
<!-- ============================================================= -->

  <xsl:template name="chkdir">
    <xsl:param name="worker"/>

    <xsl:text>/scratch/</xsl:text>
    <xsl:value-of select="$worker"/>
    <xsl:text>/</xsl:text>
  </xsl:template>

  <xsl:output method="text"/>
  
  <xsl:template match="/">
    <xsl:apply-templates select="experiment"/>
  </xsl:template>
    
  <xsl:template match="atom-list">
    <xsl:value-of select="@charge"/>
    <xsl:text>,</xsl:text>
    <xsl:value-of select="@multiplicity"/>
    <xsl:call-template name="endl"/>
    <xsl:choose>
      <xsl:when test="@format = 'axyz'">
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
        <xsl:call-template name="endl"/>
        <xsl:call-template name="make-table">
          <xsl:with-param name="list-string">
            <xsl:value-of select="substring-after(.,'&#92;&#92;')"/>
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
        <xsl:call-template name="endl"/>
      </xsl:when>
      <xsl:when test="@format = 'free'">
        <!-- Assume it is already formatted -->
        <xsl:value-of select="."/>
        <xsl:call-template name="endl"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:text>Stylesheet cannot handle format "</xsl:text>
        <xsl:value-of select="@format"/>
        <xsl:text>" atom-lists, content:</xsl:text>
        <xsl:call-template name="endl"/>
        <xsl:text>[[</xsl:text>
        <xsl:value-of select="."/>
        <xsl:text>]]</xsl:text>
        <xsl:call-template name="endl"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="calc-method">
    <xsl:param name="basename"/>
    <xsl:param name="control-lines"/>
    
    <xsl:value-of select="../calc-program/@name"/>
    <xsl:text> &lt;&lt; EOF &gt; </xsl:text>
    <xsl:value-of select="$basename"/>
    <xsl:text>.</xsl:text>
    <xsl:value-of select="@method"/>
    <xsl:text>.</xsl:text>
    <xsl:value-of select="@type"/>
    <xsl:call-template name="endl"/>
    <xsl:value-of select="$control-lines"/>
    <xsl:call-template name="endl"/>
    <!-- Generate the program task control lines -->
    <xsl:text>#P </xsl:text>    
    <xsl:value-of select="@method"/>
    <xsl:if test="@method != 'ONIOM'">
      <xsl:text> </xsl:text>
    </xsl:if>
    <xsl:value-of select="@basis-set"/>
    <xsl:text> </xsl:text>
    <xsl:apply-templates select="./option"/>
    <xsl:call-template name="endl"/>
    <xsl:text>#</xsl:text>    
    <xsl:apply-templates select="../calc-program/option" mode="general">
      <xsl:with-param name="step">
        <xsl:value-of select="@step"/>
      </xsl:with-param>
    </xsl:apply-templates>
    <xsl:if test="not(atom-list)">
      <xsl:call-template name="endl"/>
      <xsl:text>#geom(allcheck) guess(check) </xsl:text>
    </xsl:if>
    <xsl:call-template name="endl"/>
    <xsl:call-template name="endl"/>
    <xsl:apply-templates select="../information"/>
    <xsl:choose>
      <xsl:when test="@type='freq'">
        <xsl:text> - harmonic frequency calculation</xsl:text>
      </xsl:when>
      <xsl:when test="@type='opt'">
        <xsl:text> - geometry optimisation calculation</xsl:text>
      </xsl:when>
      <xsl:when test="@type='sp'">
        <xsl:text> - single point calculation</xsl:text>
      </xsl:when>
      <xsl:when test="@type='stable'">
        <xsl:text> - stability calculation</xsl:text>
      </xsl:when>
    </xsl:choose>
    <!-- Now let us encode some information for later use -->
    <xsl:call-template name="endl"/>
    <xsl:text>&lt;experiment</xsl:text>
    <xsl:call-template name="endl"/>
    <xsl:text> pageref="</xsl:text>
    <xsl:value-of select="../@pageref"/>
    <xsl:text>-</xsl:text>
    <xsl:value-of select="@type"/>
    <xsl:text>"</xsl:text>
    <xsl:call-template name="endl"/>
    <xsl:text> compound="</xsl:text>
    <xsl:value-of select="../@compound"/>
    <xsl:text>"</xsl:text>
    <xsl:call-template name="endl"/>
    <xsl:text> geometry="</xsl:text>
    <xsl:value-of select="../@geometry"/>
    <xsl:text>"</xsl:text>
    <xsl:call-template name="endl"/>
    <xsl:text> worker="</xsl:text>
    <xsl:value-of select="../@worker"/>
    <xsl:text>"/&gt;</xsl:text>
    <!-- Done -->
    <xsl:call-template name="endl"/>
    <xsl:call-template name="endl"/>
    <xsl:if test="atom-list">
      <xsl:apply-templates select="atom-list"/>
    </xsl:if>
    <xsl:call-template name="endl"/>
    <xsl:text>EOF</xsl:text>
    <xsl:call-template name="endl"/>
  </xsl:template>

  <xsl:template match="experiment">
    <xsl:variable name="basename">
      <xsl:value-of select="@compound"/>
      <xsl:text>.</xsl:text>
      <xsl:value-of select="@geometry"/>
      <xsl:text>.</xsl:text>
      <xsl:value-of select="@pageref"/>
    </xsl:variable>
    <!-- Generate the program starting lines -->
    <xsl:text>#!/bin/csh</xsl:text>
    <xsl:call-template name="endl"/>
    <xsl:call-template name="endl"/>
    <xsl:for-each select="calc-method">
      <!-- sort calculations by step ordinal -->
      <xsl:sort select="calc-method/@step"/>
      
      <xsl:call-template name="calc-method">
        <xsl:with-param name="basename">
          <xsl:value-of select="$basename"/>
        </xsl:with-param>
        <xsl:with-param name="control-lines">
          <!-- Pre-generate the program global control lines -->
          <xsl:text>%chk=</xsl:text>
          <xsl:call-template name="chkdir">
            <xsl:with-param name="worker">
              <xsl:value-of select="../@worker"/>
            </xsl:with-param>
          </xsl:call-template>
          <xsl:value-of select="$basename"/>
          <xsl:text>.chk</xsl:text>
        </xsl:with-param>
      </xsl:call-template>
      <xsl:call-template name="endl"/>
      <xsl:text>if ($status != 0) then</xsl:text>
      <xsl:call-template name="endl"/>
      <xsl:text>exit($status)</xsl:text>
      <xsl:call-template name="endl"/>
      <xsl:text>endif</xsl:text>
      <xsl:call-template name="endl"/>
      <xsl:call-template name="endl"/>
    </xsl:for-each>
  </xsl:template>

  <xsl:template match="option">
    <xsl:value-of select="@type"/>
    <xsl:if test="text()">
      <xsl:text>(</xsl:text>
      <xsl:value-of select="."/>
      <xsl:text>)</xsl:text>
    </xsl:if>
    <xsl:text> </xsl:text>
  </xsl:template>
  
  <xsl:template match="option" mode="general">
    <xsl:param name="step"/>
    <!-- Check if this option is appropriate for this step (empty
    @include means suitable for all) -->
    <xsl:if test="(not(@include) or contains(@include, $step)) and not(contains(@exclude, $step))">
      <xsl:value-of select="@type"/>
      <xsl:if test="text()">
        <xsl:text>(</xsl:text>
        <xsl:value-of select="."/>
        <xsl:text>)</xsl:text>
      </xsl:if>
      <xsl:text> </xsl:text>
    </xsl:if>
  </xsl:template>

<!-- ============================================================= -->
<!-- Helper templates                                              -->
<!-- ============================================================= -->

  <xsl:template name="make-table">
    <xsl:param name="list-string"/>
    <xsl:param name="table-width"/>
    <xsl:param name="column-divider"/>
    <xsl:param name="row-divider"/>

    <xsl:choose>
      <xsl:when test="contains($list-string,$row-divider)">
        <!-- Output the row -->
        <xsl:call-template name="make-table-by-row-column">
          <xsl:with-param  name="row-string">
            <xsl:value-of select="substring-before($list-string,$row-divider)"/>
          </xsl:with-param>
          <xsl:with-param name="column-divider">
            <xsl:value-of select="$column-divider"/>
          </xsl:with-param>
        </xsl:call-template>
        <!-- End the row -->
        <xsl:call-template name="endl"/>
        <!-- Do recursive call -->
        <xsl:call-template  name="make-table">
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
        <!-- End recursion stage, output whats left -->
        <xsl:call-template name="make-table-by-row-column">
          <xsl:with-param  name="row-string">
            <xsl:value-of select="$list-string"/>
          </xsl:with-param>
          <xsl:with-param name="column-divider">
            <xsl:value-of select="$column-divider"/>
          </xsl:with-param>
        </xsl:call-template>        
        <!-- End the row -->
        <xsl:call-template name="endl"/>
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
        <xsl:value-of select="substring-before($row-string, $column-divider)"/>
        <!-- get column divider -->
        <xsl:text>   </xsl:text>
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
        <xsl:value-of select="$row-string"/>
        <!-- no column divider -->
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="endl">
    <xsl:text>
</xsl:text>
  </xsl:template>

</xsl:stylesheet>
