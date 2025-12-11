<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
    <xsl:output method="html" indent="yes"/>

    <!-- Корневой шаблон -->
    <xsl:template match="/">
        <html>
            <head>
                <title>Отчет о тестах Google Test</title>
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
                <h1>Результаты тестов Google Test</h1>
                <!-- Обработка каждого тестового набора -->
                <xsl:for-each select="testsuites/testsuite">
                    <h2>Тестовый набор:
                        <xsl:value-of select="@name"/>
                    </h2>
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
                                    <!-- Надпись и эмодзи в зависимости от результата -->
                                    <xsl:choose>
                                        <xsl:when test="failure">
                                            Тест провален ❌
                                        </xsl:when>
                                        <xsl:otherwise>
                                            Тест пройден ✅
                                        </xsl:otherwise>
                                    </xsl:choose>
                                    <xsl:text> </xsl:text>
                                    <xsl:value-of select="@name"/>
                                </span>
                                <!-- Вывод сообщения об ошибке, если тест провален -->
                                <xsl:if test="failure">
                                    <div style="margin-left: 20px; color: red;">
                                        Ошибка:
                                        <xsl:value-of select="failure/@message"/>
                                    </div>
                                </xsl:if>
                            </li>
                        </xsl:for-each>
                    </ul>
                    <!-- Общая статистика по тестовому набору -->
                    <p>
                        Тестов: <xsl:value-of select="@tests"/>,
                        Провалов: <xsl:value-of select="@failures"/>,
                        Ошибок: <xsl:value-of select="@errors"/>,
                        Пропущено:
                        <xsl:value-of select="@skipped"/>
                    </p>
                </xsl:for-each>
                <!-- Общая статистика по всем тестам -->
                <h2>Итог</h2>
                <p>
                    Всего тестов: <xsl:value-of select="testsuites/@tests"/>,
                    Всего провалов: <xsl:value-of select="testsuites/@failures"/>,
                    Всего ошибок: <xsl:value-of select="testsuites/@errors"/>,
                    Всего пропущено:
                    <xsl:value-of select="testsuites/@skipped"/>
                </p>
            </body>
        </html>
    </xsl:template>
</xsl:stylesheet>