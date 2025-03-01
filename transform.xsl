<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  <xsl:output method="html" indent="yes"/>

  <!-- Корневой шаблон -->
  <xsl:template match="/">
    <html>
      <head>
        <title>Google Test Report</title>
        <style>
          body { font-family: Arial, sans-serif; }
          h1 { color: #333; }
          h2 { color: #555; }
          .passed { color: green; }
          .failed { color: red; }
          ul { list-style-type: none; padding: 0; }
          li { margin: 5px 0; }
        </style>
      </head>
      <body>
        <h1>Google Test Results</h1>
        <!-- Обработка каждого тестового набора -->
        <xsl:for-each select="testsuites/testsuite">
          <h2>Test Suite: <xsl:value-of select="@name"/></h2>
          <ul>
            <!-- Обработка каждого теста -->
            <xsl:for-each select="testcase">
              <li>
                <span>
                  <xsl:attribute name="class">
                    <xsl:choose>
                      <xsl:when test="failure">failed</xsl:when>
                      <xsl:otherwise>passed</xsl:otherwise>
                    </xsl:choose>
                  </xsl:attribute>
                  <xsl:value-of select="@name"/>
                </span>
                <!-- Вывод сообщения об ошибке, если тест провален -->
                <xsl:if test="failure">
                  <div style="margin-left: 20px; color: red;">
                    <xsl:value-of select="failure/@message"/>
                  </div>
                </xsl:if>
              </li>
            </xsl:for-each>
          </ul>
          <!-- Общая статистика по тестовому набору -->
          <p>
            Tests: <xsl:value-of select="@tests"/>, 
            Failures: <xsl:value-of select="@failures"/>, 
            Errors: <xsl:value-of select="@errors"/>, 
            Skipped: <xsl:value-of select="@skipped"/>
          </p>
        </xsl:for-each>
        <!-- Общая статистика по всем тестам -->
        <h2>Summary</h2>
        <p>
          Total Tests: <xsl:value-of select="testsuites/@tests"/>, 
          Total Failures: <xsl:value-of select="testsuites/@failures"/>, 
          Total Errors: <xsl:value-of select="testsuites/@errors"/>, 
          Total Skipped: <xsl:value-of select="testsuites/@skipped"/>
        </p>
      </body>
    </html>
  </xsl:template>
</xsl:stylesheet>
