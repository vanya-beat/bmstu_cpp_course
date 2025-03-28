<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  <xsl:output method="html" indent="yes"/>

  <xsl:template match="/">
    <html lang="ru">
      <head>
        <meta charset="UTF-8"/>
        <title>Отчет о тестировании</title>
        <style>
          :root {
            --success: #4CAF50;
            --danger: #F44336;
            --warning: #FFC107;
            --info: #2196F3;
            --dark: #343A40;
            --light: #F8F9FA;
          }
          
          body {
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            line-height: 1.6;
            color: #333;
            max-width: 1200px;
            margin: 0 auto;
            padding: 20px;
            background-color: #f5f5f5;
          }
          
          header {
            background: linear-gradient(135deg, #6B73FF 0%, #000DFF 100%);
            color: white;
            padding: 2rem;
            border-radius: 8px;
            margin-bottom: 2rem;
            box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);
          }
          
          h1 {
            margin: 0;
            font-size: 2.5rem;
          }
          
          h2 {
            color: #2c3e50;
            border-bottom: 2px solid #eee;
            padding-bottom: 0.5rem;
            margin-top: 2rem;
          }
          
          .test-suite {
            background: white;
            border-radius: 8px;
            padding: 1.5rem;
            margin-bottom: 1.5rem;
            box-shadow: 0 2px 4px rgba(0, 0, 0, 0.05);
            transition: transform 0.2s;
          }
          
          .test-suite:hover {
            transform: translateY(-2px);
            box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);
          }
          
          .test-list {
            padding: 0;
          }
          
          .test-case {
            padding: 0.8rem 1rem;
            margin: 0.5rem 0;
            border-radius: 4px;
            display: flex;
            align-items: center;
          }
          
          .passed {
            background-color: rgba(76, 175, 80, 0.1);
            border-left: 4px solid var(--success);
          }
          
          .failed {
            background-color: rgba(244, 67, 54, 0.1);
            border-left: 4px solid var(--danger);
          }
          
          .emoji {
            font-size: 1.2rem;
            margin-right: 10px;
            width: 24px;
            display: inline-block;
            text-align: center;
          }
          
          .test-name {
            font-weight: 500;
          }
          
          .error-message {
            background-color: #FFF3F3;
            color: #721C24;
            padding: 0.8rem;
            border-radius: 4px;
            margin-top: 0.5rem;
            font-family: monospace;
            white-space: pre-wrap;
          }
          
          .stats {
            display: flex;
            gap: 1rem;
            margin-top: 1rem;
            flex-wrap: wrap;
          }
          
          .stat-card {
            background: white;
            border-radius: 8px;
            padding: 1rem;
            flex: 1;
            min-width: 200px;
            box-shadow: 0 2px 4px rgba(0, 0, 0, 0.05);
            text-align: center;
          }
          
          .stat-value {
            font-size: 2rem;
            font-weight: bold;
            margin: 0.5rem 0;
          }
          
          .total-tests { color: var(--info); }
          .passed-tests { color: var(--success); }
          .failed-tests { color: var(--danger); }
          .skipped-tests { color: var(--warning); }
          
          footer {
            text-align: center;
            margin-top: 2rem;
            padding: 1rem;
            color: #777;
            font-size: 0.9rem;
          }
          
          @media (max-width: 768px) {
            body {
              padding: 10px;
            }
            
            .stats {
              flex-direction: column;
            }
          }
        </style>
      </head>
      <body>
        <header>
          <h1>📊 Отчет о тестировании</h1>
          <p>Результаты автоматического тестирования</p>
        </header>
        
        <div class="stats">
          <div class="stat-card">
            <div>Всего тестов</div>
            <div class="stat-value total-tests">
              <xsl:value-of select="testsuites/@tests"/>
            </div>
          </div>
          <div class="stat-card">
            <div>Пройдено</div>
            <div class="stat-value passed-tests">
              <xsl:value-of select="testsuites/@tests - testsuites/@failures - testsuites/@errors"/>
            </div>
          </div>
          <div class="stat-card">
            <div>Провалено</div>
            <div class="stat-value failed-tests">
              <xsl:value-of select="testsuites/@failures + testsuites/@errors"/>
            </div>
          </div>
          <div class="stat-card">
            <div>Пропущено</div>
            <div class="stat-value skipped-tests">
              <xsl:value-of select="testsuites/@skipped"/>
            </div>
          </div>
        </div>
        
        <xsl:for-each select="testsuites/testsuite">
          <div class="test-suite">
            <h2>🧪 Тестовый набор: <xsl:value-of select="@name"/></h2>
            <p>
              Время выполнения: <xsl:value-of select="@time"/> сек. |
              Тестов: <xsl:value-of select="@tests"/> |
              Провалов: <xsl:value-of select="@failures"/> |
              Ошибок: <xsl:value-of select="@errors"/> |
              Пропущено: <xsl:value-of select="@skipped"/>
            </p>
            
            <ul class="test-list">
              <xsl:for-each select="testcase">
                <li class="test-case {concat('test-', @name)}">
                  <xsl:attribute name="class">
                    <xsl:choose>
                      <xsl:when test="failure">test-case failed</xsl:when>
                      <xsl:otherwise>test-case passed</xsl:otherwise>
                    </xsl:choose>
                  </xsl:attribute>
                  
                  <span class="emoji">
                    <xsl:choose>
                      <xsl:when test="failure">❌</xsl:when>
                      <xsl:otherwise>✅</xsl:otherwise>
                    </xsl:choose>
                  </span>
                  
                  <div>
                    <div class="test-name">
                      <xsl:value-of select="@name"/>
                      <xsl:if test="@time"> (<xsl:value-of select="@time"/> сек)</xsl:if>
                    </div>
                    
                    <xsl:if test="failure">
                      <div class="error-message">
                        <strong>Ошибка:</strong> <xsl:value-of select="failure/@message"/>
                      </div>
                    </xsl:if>
                  </div>
                </li>
              </xsl:for-each>
            </ul>
          </div>
        </xsl:for-each>
        
        <footer>
          <p>Отчет сгенерирован автоматически • <xsl:value-of select="current-dateTime()"/></p>
        </footer>
      </body>
    </html>
  </xsl:template>
</xsl:stylesheet>