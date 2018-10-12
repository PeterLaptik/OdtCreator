#include "../include/OdtCreator.h"
#include "../include/OdtXmlNodes.h"

// Parses and finds necessary XML-nodes in created XML-files
// Initializes nodes pointers
// Returns false if cannot find nodes
bool OdtCreator::InitXmlNodes()
{
    wxXmlNode *rootNode;
    m_error = ODT_ERROR_NO_ERRORS;
    m_contentNode = NULL;
    m_styleNode = NULL;
    m_tableToAdd = NULL;
    m_formulaCounter = 0;
    m_tableCounter = 0;
    m_pictureCounter = 0;
    SetParagraphStyle(odtParaStandard);
    SetTextStyle(odtTextSimple);
    // Set start time of editing
    m_startTime = wxDateTime::Now();
    // Load content XML
    m_pathToContentXML.Clear();
    m_pathToContentXML<<m_tmpDirPath<<m_pathSeparator<<ODT_FILENEME_CONTENT;
    m_myContent = new wxXmlDocument();
    m_myContent->Load(m_pathToContentXML);

    // Find office:text
    rootNode = m_myContent->GetRoot();
    if (rootNode==NULL)
    {
        m_error = ODT_ERROR_CANNOT_PROCESS_XML;
        return false;
    }
    m_contentNode = rootNode->GetChildren();

    // Seeking for the text content
    while ((m_contentNode!=NULL) && (m_contentNode->GetName()!=NODE_officeBody))
    {
        if (m_contentNode->GetName()==NODE_officeAutoStyles)
        {
            m_styleNode = m_contentNode;
        }
        m_contentNode = m_contentNode->GetNext();
    }

    m_contentNode = m_contentNode->GetChildren();
    while ((m_contentNode!=NULL) && (m_contentNode->GetName()!=NODE_officeText))
    {
        if (m_contentNode->GetName()==NODE_officeAutoStyles)
        {
            m_styleNode = m_contentNode;
        }
        m_contentNode = m_contentNode->GetNext();
    }

    if (m_contentNode==NULL)
    {
        m_error = ODT_ERROR_CANNOT_PROCESS_XML;
        return false;
    }

    if (m_styleNode==NULL)
    {
        m_error = ODT_ERROR_CANNOT_PROCESS_XML;
        return false;
    }
    m_lastNode = NULL;

    // Load manifest XML
    m_pathToManifestXML.Clear();
    m_pathToManifestXML<<m_tmpDirPath<<m_pathSeparator<<DIR_META_INF<<m_pathSeparator<<ODT_FILENAME_MANIFEST;
    m_myManifest = new wxXmlDocument();
    m_myManifest->Load(m_pathToManifestXML);
    // Find office:text node
    m_manifestNode = m_myManifest->GetRoot();
    if (m_manifestNode==NULL)
    {
        m_error = ODT_ERROR_CANNOT_PROCESS_XML;
        return false;
    }
    //FillTableStyles(); - @excluded. Only actual styles have to be added to the content.xml
    m_styleList.Clear();
    return true;
}

// Adds a text (new paragraph)
// @content - text content
void OdtCreator::AddText(const wxString &content)
{
    wxXmlNode *childNode;
    wxXmlNode *textContent;

    if (!m_isProcessing)
    {
        m_error = ODT_ERROR_NO_DOC_IN_PROCESSING;
        return;
    }

    childNode = new wxXmlNode(wxXML_ELEMENT_NODE, NODE_text, wxEmptyString, -1);
    childNode->AddAttribute(TAG_textStyleName, m_paragraphStyle);
    textContent = new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, content, -1);
    childNode->AddChild(textContent);
    m_contentNode->AddChild(childNode);
    m_lastNode = childNode;
}

// Inserts a blank line (new paragraph)
void OdtCreator::Indent()
{
    wxXmlNode *childNode;

    if (!m_isProcessing)
    {
        m_error = ODT_ERROR_NO_DOC_IN_PROCESSING;
        return;
    }

    childNode = new wxXmlNode(wxXML_ELEMENT_NODE, NODE_text, wxEmptyString, -1);
    childNode->AddAttribute(TAG_textStyleName, m_paragraphStyle);
    m_contentNode->AddChild(childNode); // blank line
    m_lastNode = NULL; // or null (check later)
}

// Adds an empty node to content file for placing the formula
void OdtCreator::AddEmptyNode ()
{
    if (!m_isProcessing)
    {
        m_error = ODT_ERROR_NO_DOC_IN_PROCESSING;
        return;
    }

    wxXmlNode *childNode;
    childNode = new wxXmlNode(wxXML_ELEMENT_NODE, VAL_textP, wxEmptyString, -1);
    childNode->AddAttribute(TAG_textStyleName, m_paragraphStyle);
    m_contentNode->AddChild(childNode);
    m_lastNode = childNode;
}

// Makes the next text content to begin from new paragraph
void OdtCreator::NextParagraph()
{
    if (!m_isProcessing)
    {
        m_error = ODT_ERROR_NO_DOC_IN_PROCESSING;
        return;
    }

    m_lastNode = NULL;
}

// Appends text to the current paragraph
// @content - text content
void OdtCreator::AppendText (const wxString &content)
{
    wxXmlNode *childNode;
    wxXmlNode *textContent;

    if (!m_isProcessing)
    {
        m_error = ODT_ERROR_NO_DOC_IN_PROCESSING;
        return;
    }

    if (m_lastNode==NULL)
    {
        AddText(content);
        return;
    }

    childNode = new wxXmlNode(wxXML_ELEMENT_NODE, NODE_textSpan, wxEmptyString, -1);
    if (m_textStyle!=TAG_textNoStyle)
    {
        childNode->AddAttribute(TAG_textStyleName, m_textStyle);
    }
    textContent = new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, content, -1);
    childNode->AddChild(textContent);
    m_lastNode->AddChild(childNode);
}

// Appends text to the current paragraph
// @content - text content
// @style - text style
void OdtCreator::AppendText (const wxString &content, odtTEXT_STYLE style)
{
    wxXmlNode *childNode;
    wxXmlNode *textContent;

    if (!m_isProcessing)
    {
        m_error = ODT_ERROR_NO_DOC_IN_PROCESSING;
        return;
    }

    if (m_lastNode==NULL)
    {
        AddEmptyNode(); // See wxOdtFormula.cpp
    }
    childNode = new wxXmlNode(wxXML_ELEMENT_NODE, NODE_textSpan, wxEmptyString, -1);

    if (GetTextStyle(style)!=TAG_textNoStyle)
    {
        childNode->AddAttribute(TAG_textStyleName, GetTextStyle(style));
    }
    textContent = new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, content, -1);
    childNode->AddChild(textContent);
    m_lastNode->AddChild(childNode);
}

// Adds text (new paragraph)
// @content - text content
void OdtCreator::AddText(const wxString &content, odtTEXT_STYLE style)
{
    wxXmlNode *childNode;
    wxXmlNode *textContent;

    if (!m_isProcessing)
    {
        m_error = ODT_ERROR_NO_DOC_IN_PROCESSING;
        return;
    }

    childNode = new wxXmlNode(wxXML_ELEMENT_NODE, NODE_text, wxEmptyString, -1);
    childNode->AddAttribute(TAG_textStyleName, m_paragraphStyle); // Added
    m_lastNode = childNode;
    m_contentNode->AddChild(childNode);
    childNode = new wxXmlNode(wxXML_ELEMENT_NODE, NODE_textSpan, wxEmptyString, -1);

    if (GetTextStyle(style)!=TAG_textNoStyle)
    {
        childNode->AddAttribute(TAG_textStyleName, GetTextStyle(style));
    }
    else
    {
        childNode->AddAttribute(TAG_textStyleName, m_paragraphStyle);
    }

    textContent = new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, content, -1);
    childNode->AddChild(textContent);
    m_lastNode->AddChild(childNode);
}

// Sets text style
void OdtCreator::SetTextStyle(odtTEXT_STYLE style)
{
    if (!m_isProcessing)
    {
        m_error = ODT_ERROR_NO_DOC_IN_PROCESSING;
        return;
    }

    m_textStyle = GetTextStyle(style);
}

// Returns style name (wxString) that matches to defined style
wxString OdtCreator::GetTextStyle (odtTEXT_STYLE style)
{
    switch (style)
    {
    case odtTextBold:
        {
            return wxT("T1");
        }
    case odtTextItalic:
        {
            return wxT("T2");
        }
    case odtTextBoldItalic:
        {
            return wxT("T3");
        }
    case odtTextRed:
        {
            return wxT("T4");
        }
    }
    return wxT("no_style"); // if style = odtTextSimple
}

// Set paragraph style
void OdtCreator::SetParagraphStyle(odtPARAGRAPH_STYLE style)
{
    if (!m_isProcessing)
    {
        m_error = ODT_ERROR_NO_DOC_IN_PROCESSING;
        return;
    }

    switch (style)
        {
        case odtParaStandardCenter:
            {
                m_paragraphStyle = wxT("P2");
                break;
            }
        case odtParaStandardRight:
            {
                m_paragraphStyle = wxT("P3");
                break;
            }
        case odtParaStandardJustify:
            {
                m_paragraphStyle = wxT("P4");
                break;
            }
        case odtParaTitle:
            {
                m_paragraphStyle = wxT("Title");
                break;
            }
        case odtParaSubTitle:
            {
                m_paragraphStyle = wxT("Subtitle");
                break;
            }
        case odtParaHeader1:
            {
                m_paragraphStyle = wxT("Heading_20_1");
                break;
            }
        case odtParaHeader2:
            {
                m_paragraphStyle = wxT("Heading_20_2");
                break;
            }
        case odtParaHeader3:
            {
                m_paragraphStyle = wxT("Heading_20_3");
                break;
            }
        case odtParaHeader:
            {
                m_paragraphStyle = wxT("Heading");
                break;
            }
        default:
            {
                 m_paragraphStyle = wxT("P1");
            }
        }
}

// Creates table with default name
// @columns - number of columns
// @rows - number of rows
// @returns - false if no document is created or a table exists
bool OdtCreator::TableCreate(int columns, int rows)
{
    wxString tableName;
    m_error = ODT_ERROR_NO_ERRORS;

    if (!m_isProcessing)
    {
        m_error = ODT_ERROR_NO_DOC_IN_PROCESSING;
        return false;
    }

    if(m_doesTableExists)
    {
        m_error = ODT_ERROR_PROCESSING_TABLE_EXISTS;
        return false;
    }

    tableName = wxodt_TABLE_PREFIX;
    tableName<<m_tableCounter;
    m_tableCounter++;
    m_tableToAdd = new OdtTable(tableName, columns, rows);
    m_doesTableExists = true;
    return true;
}

// Creates table
// @name - table name
// @columns - number of columns
// @rows - number of rows
// @returns - false if no document is created or a table exists
bool OdtCreator::TableCreate(const wxString &name, int columns, int rows)
{
    m_error = ODT_ERROR_NO_ERRORS;

    if (!m_isProcessing)
    {
        m_error = ODT_ERROR_NO_DOC_IN_PROCESSING;
        return false;
    }

    if(m_doesTableExists)
    {
        m_error = ODT_ERROR_PROCESSING_TABLE_EXISTS;
        return false;
    }

    m_tableToAdd = new OdtTable(name, columns, rows);
    m_doesTableExists = true;
    return true;
}

// Sets table width
void OdtCreator::TableSetWidth(int width)
{
    if (!m_isProcessing)
    {
        m_error = ODT_ERROR_NO_DOC_IN_PROCESSING;
        return;
    }

    if(!m_doesTableExists)
    {
        return;
    }

    m_tableToAdd->SetWidth(width);
}

// Sets table align
// @margin - align type
void OdtCreator::TableSetAlign(odtTABLE_CELL_ALIGN margin)
{
    if (!m_isProcessing)
    {
        m_error = ODT_ERROR_NO_DOC_IN_PROCESSING;
        return;
    }

    if(!m_doesTableExists)
    {
        return;
    }

    m_tableToAdd->SetAlign(margin);
}

// Sets table column width
// @column - column number
// @width - column width
void OdtCreator::TableSetColumnWidth(int column, int width)
{
    if (!m_isProcessing)
    {
        m_error = ODT_ERROR_NO_DOC_IN_PROCESSING;
        return;
    }

    if(!m_doesTableExists)
    {
        return;
    }

    m_tableToAdd->SetColumnWidth(column, width);
}

// Deletes a table
void OdtCreator::TableDelete()
{
    if (!m_isProcessing)
    {
        m_error = ODT_ERROR_NO_DOC_IN_PROCESSING;
        return;
    }

    if (!m_doesTableExists)
    {
        m_error = ODT_ERROR_NO_TABLE_TO_DELETE;
        return;
    }

    delete m_tableToAdd;
    m_tableToAdd = NULL;
}

// Adds a cell value in the table
void OdtCreator::TableAddCellValue(int row, int column, const wxString &content)
{
    if (!m_isProcessing)
    {
        m_error = ODT_ERROR_NO_DOC_IN_PROCESSING;
        return;
    }

    if (!m_doesTableExists)
    {
        return;
    }

    m_tableToAdd->AddValue(row, column, content);
}

// Adds a cell value of the defined font/style
// @row, @column - cell position
// @content - cell value
// @align, @fontColor, @fontWeight - cell styles
void OdtCreator::TableAddCellValue(int row, int column, const wxString &content, int style)
{
    wxString myStyleName;

    if (!m_isProcessing)
    {
        m_error = ODT_ERROR_NO_DOC_IN_PROCESSING;
        return;
    }

    if (!m_doesTableExists)
        return;

    myStyleName<<ODT_TABLE_PREFIX<<style;

    if (m_styleList.Index(myStyleName)==wxNOT_FOUND)
        m_styleList.Add(myStyleName);

    if (!m_doesTableExists)
    {
        return;
    }

    m_tableToAdd->AddValue(row, column, content, myStyleName);
}

// Inserts table in the document
bool OdtCreator::InsertTable ()
{
    m_error = ODT_ERROR_NO_ERRORS;

    if (!m_isProcessing)
    {
        m_error = ODT_ERROR_NO_DOC_IN_PROCESSING;
        return false;
    }

    if (!m_doesTableExists)
    {
        m_error = ODT_ERROR_NO_TABLE_TO_WRITE;
        return false;
    }

    m_tableToAdd->WriteTable(m_styleNode, m_contentNode);
    delete m_tableToAdd;
    m_tableToAdd = NULL;
    m_doesTableExists = false;
    return true;
}

// Fill content.xml with style nodes for table cells
void OdtCreator::FillTableStyles ()
{
    wxString styleName;
    for (int i=odtCellAlignLeft; i<=odtCellAlignRight; i *= 2)
    {
        for (int j=odtCellColorBlack; j<=odtCellColorRed; j *= 2)
        {
            for (int k=odtCellFontSimple; k<=odtCellFontBoldItalic; k *= 2)
            {
                styleName = ODT_TABLE_PREFIX;
                styleName<<(i + j + k);     // unique number in the name
                if (m_styleList.Index(styleName)!=wxNOT_FOUND)        // add only if the style is being used
                    FillStyleData (i, j, k);                        // no unused styles in xml-file
            }
        }
    }
}

// Adds a combination of style nodes for tables
// @i,j,k - constants
void OdtCreator::FillStyleData (int i, int j, int k)
{
    wxString styleName;
    wxString attrValue;
    wxXmlNode *myStyleNode;
    wxXmlNode *childStyle;

    styleName.Clear();
    styleName<<ODT_TABLE_PREFIX<<(i + j + k);
    myStyleNode = new wxXmlNode(wxXML_ELEMENT_NODE, NODE_styleStyle, wxEmptyString, -1);
    myStyleNode->AddAttribute(TAG_styleName, styleName);
    myStyleNode->AddAttribute(TAG_styleFamily, VAL_paragraph);
    myStyleNode->AddAttribute(TAG_parentStyleName, VAL_table20contents);
    childStyle = new wxXmlNode(wxXML_ELEMENT_NODE, NODE_styleParagraphProperties, wxEmptyString, -1);

    // Align value
    if (i!=0)
    {
        attrValue.Clear();
        switch (i)
        {
        case odtCellAlignLeft:
            {
                attrValue = wxT("start");
                break;
            }
        case odtCellAlignCenter:
            {
                attrValue = wxT("center");
                break;
            }
        case odtCellAlignRight:
            {
                attrValue = wxT("end");
                break;
            }
        case odtCellAlignJustify:
            {
                attrValue = wxT("justify");
                break;
            }
        default:
            {
                attrValue = wxT("start");
                break;
            }
        }
        childStyle->AddAttribute(TAG_foTextAlign, attrValue);
        childStyle->AddAttribute(TAG_styleJustifySingleWord, VAL_false);
        myStyleNode->AddChild(childStyle);
        childStyle = new wxXmlNode(wxXML_ELEMENT_NODE, NODE_styleTextProperties, wxEmptyString, -1);
    }

    // Font color
    if (j!=0)
    {
        switch (j)
        {
            case odtCellColorRed:
            {
                childStyle->AddAttribute(TAG_foColor, VAL_redColor);
                break;
            }
        }
    }

    // Font weight
    if (k!=0)
    {
        switch (k)
        {
            case odtCellFontBold:
            {
                childStyle->AddAttribute(TAG_foFontWeight, VAL_bold);
                childStyle->AddAttribute(TAG_rsid, RSID);
                childStyle->AddAttribute(TAG_officeParagraphRSID, RSID);
                childStyle->AddAttribute(TAG_styleFontStyleAsian, VAL_bold);
                childStyle->AddAttribute(TAG_styleFontStyleComplex, VAL_bold);
                break;
            }
            case odtCellFontItalic:
            {
                childStyle->AddAttribute(TAG_foFontWeight, VAL_italic);
                childStyle->AddAttribute(TAG_rsid, RSID);
                childStyle->AddAttribute(TAG_officeParagraphRSID, RSID);
                childStyle->AddAttribute(TAG_styleFontStyleAsian, VAL_italic);
                childStyle->AddAttribute(TAG_styleFontStyleComplex, VAL_italic);
                break;
            }
        }
    }
    myStyleNode->AddChild(childStyle);
    m_styleNode->AddChild(myStyleNode);
}

// Unites cells (vertically)
// @startRow - first cell row position
// @startCol - first cell column position
// @number - number of cells to be united
void OdtCreator::TableSpanRows(int startRow, int startCol, int number)
{
    if (!m_isProcessing)
    {
        m_error = ODT_ERROR_NO_DOC_IN_PROCESSING;
        return;
    }

    if (!m_doesTableExists)
        return;

    m_tableToAdd->SpanRows(startRow, startCol, number);
}

// Unites cells (horizontally)
// @startRow - first cell row position
// @startCol - first cell column position
// @number - number of cells to be united
void OdtCreator::TableSpanColumns(int startRow, int startCol, int number)
{
    if (!m_isProcessing)
    {
        m_error = ODT_ERROR_NO_DOC_IN_PROCESSING;
        return;
    }

    if (!m_doesTableExists)
        return;

    m_tableToAdd->SpanColumns(startRow, startCol, number);
}

// Sets table borders visibility
// @hasBorders - true to set borders visible / false - to set borders invisible
void OdtCreator::TableSetBorders(bool hasBorders)
{
    if (!m_isProcessing)
    {
        m_error = ODT_ERROR_NO_DOC_IN_PROCESSING;
        return;
    }

    if (!m_doesTableExists)
        return;

    m_tableToAdd->SetBorders(hasBorders);
}
