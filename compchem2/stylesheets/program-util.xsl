<?xml version="1.0" ?>
<xsl:stylesheet version="1.0"
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<!-- ============================================================= -->
<!--
   Utility Stylesheet

   This stylesheet provides a number of utility styles that are
   general to all other stylesheets that operate on "program.dtd"
   files. This includes operations that operate across documents.

   Date: 28 January 2003
   Date: $Date: 2008/06/24 18:00:28 $

   $Log: program-util.xsl,v $
   Revision 1.5  2008/06/24 18:00:28  finnerty
   Last commit for version 1.0

   Revision 1.4  2008/01/15 20:56:46  finnerty
   Updated for program.dtd v3

   Revision 1.3  2008/01/13 18:38:19  finnerty
   Updated for program.dtd v3

   Revision 1.2  2008/01/13 17:55:40  finnerty
   Updated for program.dtd v3

   Revision 1.2  2008/01/12 12:38:05  finnerty
   Initial version

   Revision 1.1  2008/01/12 12:37:22  finnerty
   Initial version


   $Revision: 1.5 $

                                                                   -->
<!-- ============================================================= -->
  <xsl:template name="replaceAllSubStrings">
    <xsl:param name="subString"/>
    <xsl:param name="replaceString"/>
    <xsl:param name="stringToSearchIn"/>
    <xsl:choose>
      <xsl:when test="contains($stringToSearchIn,$subString)">
        <xsl:value-of select="substring-before($stringToSearchIn,$subString)"/>
        <xsl:value-of select="$replaceString"/>
        <xsl:call-template name="replaceAllSubStrings">
          <xsl:with-param name="stringToSearchIn">
            <xsl:value-of select="substring-after($stringToSearchIn,$subString)"/>
          </xsl:with-param>
          <xsl:with-param name="subString">
            <xsl:value-of  select="$subString"/>
          </xsl:with-param>
          <xsl:with-param name="replaceString">
            <xsl:value-of select="$replaceString"/>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$stringToSearchIn"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <!--
    Template to automate jumping proj-src.  This method allows
    up to three paramaters to be transferred. 
  -->
  <xsl:template match="proj-src">
    <xsl:param name="p1"/>
    <xsl:param name="p2"/>
    <xsl:param name="p3"/>
    <xsl:param name="label" select="@label"/>
    <!-- <xsl:message terminate="no">
	 <xsl:text>IN UTIL "proj-src" template: </xsl:text>
	 <xsl:value-of select="@href"/>
	 </xsl:message> -->
    <xsl:apply-templates select="document(@href)//project[@label=$label]">
      <xsl:with-param name="p1" select="$p1"/>
      <xsl:with-param name="p2" select="$p2"/>
      <xsl:with-param name="p3" select="$p3"/>
    </xsl:apply-templates>
  </xsl:template>

  <!--
    Template to automate jumping memo-src.  This method allows
    up to three paramaters to be transferred. 
  -->
  <xsl:template match="memo-src">
    <xsl:param name="p1"/>
    <xsl:param name="p2"/>
    <xsl:param name="p3"/>
    <xsl:param name="label" select="@label"/>
    <!-- <xsl:message terminate="no">
	 <xsl:text>IN UTIL "memo-src" template: </xsl:text>
	 <xsl:value-of select="@href"/>
	 </xsl:message> -->
    <xsl:apply-templates select="document(@href)//memo[@label=$label]">
      <xsl:with-param name="p1" select="$p1"/>
      <xsl:with-param name="p2" select="$p2"/>
      <xsl:with-param name="p3" select="$p3"/>
    </xsl:apply-templates>
  </xsl:template>

  <!--
    The following template walks the document in compound/formula order
    for the project with @label = $project-name calling memo templates.
    Parameters are:
      project-name = label of project to use
      p1 = passed as parameter p1 to {template match="memo"} call

    This template also calls templates named "compound-start", "compound-end",
    "formula-start" and "formula-end" with parameter p1 at appropriate
    places.

    At each formula node the template eventually calls to a template with 
    match="memo" with three parameters and where memo/@compound=[compound/@label]
    and memo/@formula=[formula/@label]:
      p1 = supplied in call to compound-formula-walk
      p2 = compound-label
      p3 = formula-label
    
    Example usage (in pseudo code) to get 
    call-template [p1=freq-mode]
    call-template [p1=thermo-mode]

    template (memo)
      apply-template (datum) mode=$p1  
   
    template (datum) mode=freq-mode
      ...
    
    template (datum) mode=thermo-mode
      ...
  -->
  <xsl:template name="compound-formula-memo-walk">
    <xsl:param name="p1"/>
    <xsl:param name="project-name"/>
    <xsl:for-each select="compound">
      <xsl:call-template name="compound-start">
        <xsl:with-param name="p1" select="$p1"/>
      </xsl:call-template>
      <xsl:param name="p2" select="@label"/>
      <xsl:for-each select="formula">
        <xsl:call-template name="formula-start">
          <xsl:with-param name="p1" select="$p1"/>
        </xsl:call-template>
        <xsl:variable name="project-node" select="(//project[@label=$project-name]/experiment[memo[@compound=$p2 and @formula=$p3]|memo-src[@compound=$p2 and @formula=$p3]]|document(//proj-src[@label=$project-name]/@href)//project[@label=$project-name]/experiment[memo[@compound=$p2 and @formula=$p3]|memo-src[@compound=$p2 and @formula=$p3]])"/>    
        <xsl:if test="0=count($project-node)">
          <xsl:message terminate="yes">
  	    <xsl:text>No project found in compound-formula-memo-visit with project: "</xsl:text>
  	    <xsl:value-of select="$project-name"/>
  	    <xsl:text>", compound "</xsl:text>
  	    <xsl:value-of select="$p2"/>
  	    <xsl:text>", formula "</xsl:text>
  	    <xsl:value-of select="$p3"/>
  	    <xsl:text>", in "</xsl:text>
  	    <xsl:value-of select="$p1"/>
  	    <xsl:text>"</xsl:text>
            <xsl:text>
</xsl:text>
  	  </xsl:message>
        </xsl:if>
        <xsl:for-each select="$p1">
          <xsl:variable name="p1" select="."/>
          <!-- <xsl:message exit="no">In visit p1[i] = <xsl:value-of select="."/>
</xsl:message> -->
          <xsl:call-template name="visit-start">
            <xsl:with-param name="p1" select="$p1"/>
          </xsl:call-template>
          <xsl:apply-templates select="$project-node" mode="compound-formula-walk">
            <xsl:with-param name="p1" select="$p1"/>
            <xsl:with-param name="p2" select="$p2"/>
            <xsl:with-param name="p3" select="$p3"/>
          </xsl:apply-templates>
          <xsl:call-template name="visit-end">
            <xsl:with-param name="p1" select="$p1"/>
          </xsl:call-template>
        </xsl:for-each>
        <xsl:call-template name="formula-end">
          <xsl:with-param name="p1" select="$p1"/>
        </xsl:call-template>
      </xsl:for-each>
      <xsl:call-template name="compound-end">
        <xsl:with-param name="p1" select="$p1"/>
      </xsl:call-template>
    </xsl:for-each>
  </xsl:template>

  <!--
    The following template walks the document in compound/formula order
    for the project with @label = $project-name calling memo templates.
    Parameters are:
      project-name = label of project to use
      p1 = list of parameters

    This template also calls templates named "compound-start", "compound-end",
    "formula-start", "formula-end" with no parameters and "visit-start" and
    "visit-end" with parameter p1[i] at appropriate places.

    At each formula node the template steps along the list of nodes in p1 and
    eventually calls to a template with match="memo" where memo/@compound=[compound/@label]
    and memo/@formula=[formula/@label] with three parameters:
      p1 = p1[i] supplied in call to compound-formula-walk
      p2 = experiment/@reference
      p3 = experiment/@worker
    
    Example usage (in pseudo code) to get 
    call-template [p1=document('somefile')//nodes]

    template (memo)
      apply-template (datum) mode=$p1  
   
    template (datum) mode=freq-mode
      ...
    
    template (datum) mode=thermo-mode
      ...
  -->
  <xsl:template name="compound-formula-memo-visit">
    <xsl:param name="p1"/>
    <xsl:param name="project-name"/>
    <!-- <xsl:message exit="no">In visit p1 = <xsl:value-of select="$p1"/>
</xsl:message> -->
    <xsl:for-each select="//compound">
      <!-- <xsl:message exit="no">In visit compound = <xsl:value-of select="@label"/>
</xsl:message> -->
      <xsl:call-template name="compound-start"/>
      <xsl:variable name="p2" select="@label"/>
      <xsl:for-each select="formula">
        <xsl:variable name="p3" select="@label"/>
        <!-- <xsl:message exit="no">In visit formula = <xsl:value-of select="@label"/>
</xsl:message> -->
        <xsl:call-template name="formula-start"/>
        <xsl:variable name="project-node" select="(//project[@label=$project-name]/experiment[memo[@compound=$p2 and @formula=$p3]|memo-src[@compound=$p2 and @formula=$p3]]|document(//proj-src[@label=$project-name]/@href)//project[@label=$project-name]/experiment[memo[@compound=$p2 and @formula=$p3]|memo-src[@compound=$p2 and @formula=$p3]])"/>    
        <xsl:if test="0=count($project-node)">
          <xsl:message terminate="yes">
  	    <xsl:text>No project found in compound-formula-memo-visit with project: "</xsl:text>
  	    <xsl:value-of select="$project-name"/>
  	    <xsl:text>", compound "</xsl:text>
  	    <xsl:value-of select="$p2"/>
  	    <xsl:text>", formula "</xsl:text>
  	    <xsl:value-of select="$p3"/>
  	    <xsl:text>", in "</xsl:text>
  	    <xsl:value-of select="$p1"/>
  	    <xsl:text>"
</xsl:text>
  	  </xsl:message>
        </xsl:if>
        <xsl:for-each select="$p1">
          <xsl:variable name="p1" select="."/>
          <!-- <xsl:message exit="no">In visit p1[i] = <xsl:value-of select="."/>
</xsl:message> -->
          <xsl:call-template name="visit-start">
            <xsl:with-param name="p1" select="$p1"/>
          </xsl:call-template>
          <xsl:apply-templates select="$project-node" mode="compound-formula-walk">
            <xsl:with-param name="p1" select="$p1"/>
            <xsl:with-param name="p2" select="$p2"/>
            <xsl:with-param name="p3" select="$p3"/>
          </xsl:apply-templates>
          <xsl:call-template name="visit-end">
            <xsl:with-param name="p1" select="$p1"/>
          </xsl:call-template>
        </xsl:for-each>
        <xsl:call-template name="formula-end"/>
      </xsl:for-each>
      <xsl:call-template name="compound-end"/>
    </xsl:for-each>
  </xsl:template>

  <xsl:template match="experiment" mode="compound-formula-walk">
    <xsl:param name="p1"/>
    <xsl:param name="p2"/>
    <xsl:param name="p3"/>
    <xsl:call-template name="experiment-start">
      <xsl:with-param name="p1" select="$p1"/>
    </xsl:call-template>
    <xsl:apply-templates select="memo[@compound=$p2 and @formula=$p3]|memo-src[@compound=$p2 and @formula=$p3]">
      <xsl:with-param name="p1" select="$p1"/>
      <xsl:with-param name="p2" select="@reference"/>
      <xsl:with-param name="p3" select="@worker"/>
    </xsl:apply-templates>
    <xsl:call-template name="experiment-end">
      <xsl:with-param name="p1" select="$p1"/>
    </xsl:call-template>
  </xsl:template>
  
</xsl:stylesheet>
