<?xml version="1.0"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
<xsl:output method="xml" encoding="UTF-8"/>
<xsl:variable name="merge_items" select="document($merge_file)/results/errors/error" />

<!-- A default template that makes an XML output identical to the input -->
<xsl:template match="@* | node()">
  <xsl:copy>
    <xsl:apply-templates select="@* | node()"/>
  </xsl:copy>
</xsl:template>

<!--
A specific template for the "errors" element, which includes:
    all the "error" child elements from this element
    all the "error" elements from the merged document
-->
<xsl:template match="results/errors">
  <xsl:copy>
    <xsl:apply-templates select="error" />
    <xsl:apply-templates select="$merge_items" />
  </xsl:copy>
</xsl:template>

</xsl:stylesheet>
