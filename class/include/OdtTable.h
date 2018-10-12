#ifndef WXODTTABLE_H_INCLUDED
#define WXODTTABLE_H_INCLUDED

#include <wx/string.h>
#include <wx/xml/xml.h>

// The class describes a table that has to be inserted into a document
// The table is created via wxOdtCreator-class

// Cell style values
enum odtTABLE_CELL_STYLE
{
    odtCellAlignLeft = 1,
    odtCellAlignCenter = 2,
    odtCellAlignJustify = 4,
    odtCellAlignRight = 8,
    odtCellColorBlack = 16,
    odtCellColorRed = 32,
    odtCellFontSimple = 64,
    odtCellFontBold = 128,
    odtCellFontItalic = 256,
    odtCellFontBoldItalic = 512
};

// table cell align values
enum odtTABLE_CELL_ALIGN
{
    odtTableAlignMargins,
    odtTableAlignLeft,
    odtTableAlignRight,
    odtTableAlignCenter
};

class OdtTable
{
public:
    OdtTable (const wxString &name, int cols, int rows);
    virtual ~OdtTable();
    void SetWidth(int width);
    void SetAlign(odtTABLE_CELL_ALIGN align);
    void SetColumnWidth(int column, int width);
    void AddValue(int row, int column, const wxString cellValue);
    void AddValue(int row, int column, const wxString &cellValue, const wxString &styleName);
    void AddCellFormula(int row, int column, wxXmlNode *formula, const wxString &myStyleName);
    void SpanColumns(int startRow, int startCol, int number);
    void SpanRows(int startRow, int startCol, int number);
    void SetBorders(bool hasBorder);
    void WriteTable(wxXmlNode *style, wxXmlNode *content);

protected:

private:
    void MakeNodes(void);
    void FillAll(void);
    wxString GetAlign(short align);

    wxXmlNode *m_tableStyleFirstNode;   // root node
    wxXmlNode *m_tableContentRootNode;  // node for cells content
    wxXmlNode **m_cellsArray;           // don't remember this but it works
    wxXmlNode *m_tableColStyles;        // column style node
    wxXmlNode **m_tableStyleNodes;      // array for cells style content
    wxXmlNode **m_tableContentNodes;    // array for cells content
    wxXmlNode *m_nodeToAdd;             // temporary xml node
    wxString m_myName;          // table name
    int m_rowsNumber;       // max rows
    int m_colsNumber;       // max columns
    int m_width;            // table width
    short m_align;          // align type for cell
    bool m_border;          // is border visible
    bool m_wasNodeAdded;    // have any nodes been created
};

#endif // WXODTTABLE_H_INCLUDED
