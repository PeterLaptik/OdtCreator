#include "../include/OdtTable.h"
#include "../include/OdtXmlNodes.h"

const int MAX_COLUMNS = 20;
const int MAX_ROWS = 1000;

OdtTable::OdtTable (const wxString &name, int cols, int rows)
{
    m_myName = name;
    m_colsNumber = cols;
    m_rowsNumber = rows;
    m_border = true;
    m_align = odt_ALIGN_DEFAULT;
    m_width = odt_WIDTH_DEFAULT;
    if (cols>MAX_COLUMNS) {m_colsNumber = MAX_COLUMNS;}
    if (rows>MAX_ROWS) {m_rowsNumber = MAX_ROWS;}
    MakeNodes();
}

OdtTable::~OdtTable()
{
    if (!m_wasNodeAdded)
    {
        // Delete styles
        delete m_tableStyleFirstNode;
        delete[] m_tableColStyles;
        for (int i=0; i<m_colsNumber; i++)
        {
            delete[] m_tableStyleNodes[i];
        }
        delete[] m_tableStyleNodes;

        // Delete content
        delete m_tableContentRootNode;
        for (int i=0; i<m_colsNumber; i++)
        {
            delete[] m_cellsArray[i];
        }
        delete[] m_cellsArray;
    }
}

// Creates nodes arrays for styles and contents
void OdtTable::MakeNodes ()
{
    wxString columnName;

    // Content root node
    m_tableContentRootNode = new wxXmlNode(wxXML_ELEMENT_NODE, NODE_tableName, wxEmptyString, -1);
    m_tableContentRootNode->AddAttribute(TAG_tableName, m_myName);
    m_tableContentRootNode->AddAttribute(TAG_styleName, m_myName);
    for (int i=0; i<m_colsNumber; i++)
    {
        m_nodeToAdd = new wxXmlNode(wxXML_ELEMENT_NODE, NODE_tableColumn, wxEmptyString, -1);
        columnName.Clear();
        columnName<<m_myName<<mDot<<(char)(65 + i);                                               // ASCII chars
        m_nodeToAdd->AddAttribute(TAG_tableStyleName, columnName);
        m_tableContentRootNode->AddChild(m_nodeToAdd);
    }
    columnName.Clear();
    columnName<<m_colsNumber;

    // Create array of content
    m_cellsArray = new wxXmlNode*[m_colsNumber];
    for (int i=0; i<m_colsNumber; i++)
    {
        m_cellsArray[i] = new wxXmlNode[m_rowsNumber];
    }
    for (int i=0; i<m_colsNumber; i++)
    {
        for (int j=0; j<m_rowsNumber; j++)
        {
            m_cellsArray[i][j].SetType(wxXML_ELEMENT_NODE);
            m_cellsArray[i][j].SetName(NODE_tableCell);
            m_cellsArray[i][j].SetContent(wxEmptyString);
            columnName.Clear();
            columnName<<m_myName<<mDot<<(char)(65 + i)<<j+1;                                      // ASCII chars
            m_cellsArray[i][j].AddAttribute(TAG_tableStyleName, columnName);
            m_cellsArray[i][j].AddAttribute(TAG_valueType, VAL_string);
        }
    }

    // Style root node
    m_tableStyleFirstNode = new wxXmlNode(wxXML_ELEMENT_NODE, NODE_styleStyle, wxEmptyString, -1);
    m_tableStyleFirstNode->AddAttribute(TAG_styleName, m_myName);
    m_tableStyleFirstNode->AddAttribute(TAG_styleFamily, VAL_table);

    // Style matrices
    m_tableColStyles = new wxXmlNode[m_colsNumber];
    m_tableStyleNodes = new wxXmlNode*[m_colsNumber];
    for (int i=0; i<m_colsNumber; i++)
    {
        m_tableStyleNodes[i] = new wxXmlNode[m_rowsNumber];
    }

    // Initial arrays filling
    for (int i=0; i<m_colsNumber; i++)
    {
        for (int j=0; j<m_rowsNumber; j++)
        {
            // Style
            m_tableStyleNodes[i][j].SetType(wxXML_ELEMENT_NODE);
            m_tableStyleNodes[i][j].SetName(TAG_styleStyle);
            m_tableStyleNodes[i][j].SetContent(wxEmptyString);
            columnName.Clear();
            columnName<<m_myName<<mDot<<(char)(65 + i)<<j+1;                                      // ASCII chars
            m_tableStyleNodes[i][j].AddAttribute(TAG_styleName, columnName);
            m_tableStyleNodes[i][j].AddAttribute(TAG_styleFamily, VAL_tableCell);
        }
        m_tableColStyles[i].SetType(wxXML_ELEMENT_NODE);
        m_tableColStyles[i].SetName(TAG_styleStyle);
        m_tableColStyles[i].SetContent(wxEmptyString);
        columnName.Clear();
        columnName<<m_myName<<mDot<<(char)(65 + i);                                               // ASCII chars
        m_tableColStyles[i].AddAttribute(TAG_styleName, columnName);
        m_tableColStyles[i].AddAttribute(TAG_styleFamily, VAL_tableColumn);
    }

    // Here table style has to be added
    m_wasNodeAdded = false;
}

// Sets table width
void OdtTable::SetWidth (int width)
{
    m_width = width;
}

// Sets table align
void OdtTable::SetAlign (odtTABLE_CELL_ALIGN tableAlign)
{
    if ((tableAlign>odtTableAlignCenter) || (tableAlign<0))
    {
        m_align = odtTableAlignMargins;
    }
    else
    {
        m_align = tableAlign;
    }
}

// Sets column width
// @column - column number
// @width - width
void OdtTable::SetColumnWidth (int column, int width)
{
    wxString strVal;
    m_nodeToAdd = new wxXmlNode(wxXML_ELEMENT_NODE, NODE_tabColProps, wxEmptyString, -1);
    strVal.Clear();
    strVal<<width;
    strVal<<mUnits;
    m_nodeToAdd->AddAttribute(TAG_styleColumnWidth, strVal);
    m_tableColStyles[column].AddChild(m_nodeToAdd);
}

// Fills styles for nodes array
void OdtTable::FillAll()
{
    for (int i=0; i<m_colsNumber; i++)
    {
        for (int j=0; j<m_rowsNumber; j++)
        {
            m_nodeToAdd = new wxXmlNode(wxXML_ELEMENT_NODE, NODE_tabCelProps,
                                      wxEmptyString, -1);
            m_nodeToAdd->AddAttribute(TAG_fooPadding, mMargVal);

            if (m_border)
            {
                m_nodeToAdd->AddAttribute(TAG_fooBorder, VAL_borderVal);
            }

            m_tableStyleNodes[i][j].AddChild(m_nodeToAdd);
        }
    }
}

// Inserts nodes into content XML
// (Adds table in the text)
void OdtTable::WriteTable(wxXmlNode *style, wxXmlNode *content)
{
    wxString strToAdd;

    FillAll();

    // Table styles
    m_nodeToAdd = new wxXmlNode(wxXML_ELEMENT_NODE, NODE_styleTableProps, wxEmptyString, -1);
    strToAdd.Clear();
    strToAdd<<m_width<<mUnits;
    m_nodeToAdd->AddAttribute(TAG_styleWidth, strToAdd);
    m_nodeToAdd->AddAttribute(TAG_tableAlign, GetAlign(m_align));
    m_tableStyleFirstNode->AddChild(m_nodeToAdd);
    style->AddChild(m_tableStyleFirstNode);

    for (int i=0; i<m_colsNumber; i++)
    {
        style->AddChild(&m_tableColStyles[i]);
    }

    for (int i=0; i<m_colsNumber; i++)
    {
        for (int j=0; j<m_rowsNumber; j++)
        {
            style->AddChild(&m_tableStyleNodes[i][j]);
        }
    }

    // Adds table cell's content
    for (int j=0; j<m_rowsNumber; j++)
    {
        m_nodeToAdd = new wxXmlNode(wxXML_ELEMENT_NODE, TAG_tableRow, wxEmptyString, -1);
        for (int i=0; i<m_colsNumber; i++)
        {
            m_nodeToAdd->AddChild(&m_cellsArray[i][j]);
        }
        m_tableContentRootNode->AddChild(m_nodeToAdd);
    }

    content->AddChild(m_tableContentRootNode);
    m_wasNodeAdded = true;
}

// Adds a cell value into the table
// @row - number of row
// @column - number of column
// @cellValue - value
void OdtTable::AddValue(int row, int column, wxString cellValue)
{
    wxXmlNode *textContent;

    if ((row>=m_rowsNumber)||(column>=m_colsNumber))
    {
        return;
    }

    m_nodeToAdd = new wxXmlNode(wxXML_ELEMENT_NODE, TAG_textP, wxEmptyString, -1);
    m_nodeToAdd->AddAttribute(TAG_textStyleName, TAG_tableFont);
    textContent = new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, cellValue, -1);
    m_nodeToAdd->AddChild(textContent);
    m_cellsArray[column][row].AddChild(m_nodeToAdd);
}

// Adds a cell value into the table with defined style
// @row - number of row
// @column - number of column
// @cellValue - value
// @styleName - style name (integer value)
void OdtTable::AddValue(int row, int column, const wxString &cellValue, const wxString &styleName)
{
    wxXmlNode *textContent;

    if ((row>=m_rowsNumber)||(column>=m_colsNumber))
    {
        return;
    }

    m_nodeToAdd = new wxXmlNode(wxXML_ELEMENT_NODE, TAG_textP, wxEmptyString, -1);
    m_nodeToAdd->AddAttribute(TAG_textStyleName, styleName);
    textContent = new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, cellValue, -1);
    m_nodeToAdd->AddChild(textContent);
    m_cellsArray[column][row].AddChild(m_nodeToAdd);
}

// Adds a formula object into the cell
// @row and @column - cell position
// @formula - formula node to add
void OdtTable::AddCellFormula(int row, int column,
                                wxXmlNode *formula, const wxString &myStyleName)
{
    wxString voidStyle = ODT_TABLE_PREFIX;
    voidStyle<<-1;

    if ((row>=m_rowsNumber)||(column>=m_colsNumber))
    {
        return;
    }

    m_nodeToAdd = new wxXmlNode(wxXML_ELEMENT_NODE, TAG_textP, wxEmptyString, -1);

    if (myStyleName==voidStyle)
    {
        m_nodeToAdd->AddAttribute(TAG_textStyleName, TAG_tableFont);
    }
    else
    {
        m_nodeToAdd->AddAttribute(TAG_textStyleName, myStyleName);
    }

    m_nodeToAdd->AddChild(formula);
    m_cellsArray[column][row].AddChild(m_nodeToAdd);
}

// Returns align value as a tag value
// Returns 'margins' by default
// @align - align-type constant
wxString OdtTable::GetAlign (short align)
{
    switch (align)
    {
    case odtTableAlignLeft:
        {
            return wxT("left");
            break;
        }
    case odtTableAlignRight:
        {
            return wxT("right");
            break;
        }
    case odtTableAlignCenter:
        {
            return wxT("center");
            break;
        }
    }
    return wxT("margins");
}

// Unites columns
// Do nothing if the values are out of table
// @startRow - row number
// @startCol - column number
// @number - number of columns to be united
void OdtTable::SpanColumns(int startRow, int startCol, int number)
{
    if (m_colsNumber<(startCol + number))
        return;

    if ((startRow>=m_rowsNumber) || (startCol>=m_colsNumber))
        return;

    m_cellsArray[startCol][startRow].AddAttribute(TAG_tableColSpan, wxString::Format(wxT("%i"), number));
}

// Unites rows
// Do nothing if the values are out of table
// @startRow - row number
// @startCol - column number
// @number - number of rows to be united
void OdtTable::SpanRows(int startRow, int startCol, int number)
{
    if ((startRow>=m_rowsNumber) || (startCol>=m_colsNumber))
        return;

    if (m_rowsNumber<(startRow + number))
        return;

    m_cellsArray[startCol][startRow].AddAttribute(TAG_tableRowSpan, wxString::Format(wxT("%i"), number));
}

// Sets whether borders have to be shown
// hasBorder - true for solid borders / false - for hidden borders
void OdtTable::SetBorders(bool hasBorder)
{
    m_border = hasBorder;
}

