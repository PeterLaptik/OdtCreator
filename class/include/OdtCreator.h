#ifndef WXODTCREATOR_H
#define WXODTCREATOR_H

/***************************************************************************************************
* A class for creating Open Document Text files (odt-extension) using wxWidgets library.           *
* Implements simple text styles, tables, images and formulas.                                      *
* Requirements: uses wxWidgets v.3.0/3.1 library classes (wxXmlDocument, wxXmlNode, wxString,      *
*               wxArrayString, wxDir, wxFileName, wxDirTraverser, wxZipEntry and streams)          *
* Author: Peter Laptik                                                                             *
* e-mail: Peter.Laptik@gmail.com                                                                   *
* License: BSD                                                                                     *
****************************************************************************************************/

#include "OdtTable.h"
#include <wx/string.h>
#include <wx/dir.h>
#include <wx/filename.h>
#include <wx/xml/xml.h>

// Error codes
enum odtERROR_CODES
{
    ODT_ERROR_NO_ERRORS,
    ODT_ERROR_FILE_EXISTS,
    ODT_ERROR_TMP_FOLDER_EXISTS,
    ODT_ERROR_COULD_NOT_MAKE_DIR,
    ODT_ERROR_NO_DOC_IN_PROCESSING,
    ODT_ERROR_CANNOT_PROCESS_XML,
    ODT_ERROR_PROCESSING_TABLE_EXISTS,
    ODT_ERROR_NO_TABLE_TO_WRITE,
    ODT_ERROR_NO_TABLE_TO_DELETE,
    ODT_ERROR_FILE_IS_PROCESSING,
    ODT_ERROR_CANNOT_BUILD_FILE,
    ODT_ERROR_WRONG_FILEPATH
};

// Paragraph styles
enum odtPARAGRAPH_STYLE
{
    odtParaStandard,
    odtParaStandardCenter,
    odtParaStandardRight,
    odtParaStandardJustify,
    odtParaTitle,
    odtParaSubTitle,
    odtParaHeader1,
    odtParaHeader2,
    odtParaHeader3,
    odtParaHeader
};

// Text styles
enum odtTEXT_STYLE
{
    odtTextSimple,
    odtTextBold,
    odtTextItalic,
    odtTextBoldItalic,
    odtTextRed
};

const int odtNO_STYLE = -1;

// Note:
// See cell styles for tables in wxOdtTable.h

class OdtCreator
{
    public:
        OdtCreator();
        OdtCreator(const wxString &myPath, const wxString &m_myName);
        OdtCreator(const wxString &fullPath);
        virtual ~OdtCreator();
        bool SetPath(const wxString &myPath);
        bool SetName(const wxString &m_myName);
        bool Init(void);                        // Initialize document
        short GetError(void);                   // Get last error code

        // Text processing
        void AddText(const wxString &content);      // Add new text: new paragraph is being created
        void AddText(const wxString &content, odtTEXT_STYLE odtTextStyle);
        void AppendText (const wxString &content);  // Append text: no new paragraph is being created
        void AppendText (const wxString &content, odtTEXT_STYLE odtTextStyle);
        void SetParagraphStyle(odtPARAGRAPH_STYLE odtParagraphStyle);
        void SetTextStyle(odtTEXT_STYLE style);
        void NextParagraph(void);       // Go to the next paragraph
        void Indent(void);              // Insert a blank line
        bool Write(void);   // Write and close the document
        void Close(void);   // Close the document without writing
        // Formula processing
        void InsertFormula (const wxString &content);
        void TableCellInsertFormula (int row, int column,
                                     const wxString &content, int style = odtNO_STYLE);
        // Picture processing
        void InsertPicture (const wxString &path, int width = 17, int height = 17);
        // Table processing
        bool TableCreate(const wxString &name, int columns, int rows);
        bool TableCreate(int columns, int rows);
        void TableDelete(void); // Delete table without inserting
        bool InsertTable(void);
        void TableSetWidth(int width);
        void TableSetAlign(odtTABLE_CELL_ALIGN margin);
        void TableSetColumnWidth(int column, int width);
        void TableAddCellValue(int row, int column, const wxString &content);
        void TableAddCellValue(int row, int column, const wxString &content, int style);
        void TableSpanColumns(int startRow, int startCol, int number);
        void TableSpanRows(int startRow, int startCol, int number);
        void TableSetBorders(bool hasBorders);

    protected:

    private:
        void ClearError(void);          // Reset last error
        void MakeStyles(void);          // Create 'styles.xml' file
        void MakeTemplates(void);       // Makes template inner structure
        void ProcessMetaXml(void);      // Changes meta.xml data
        bool InitXmlNodes(void);                        // Initialize pointers
        void FormulaToManifest(const wxString &name);   // Add formula to manifest-inf
        void AddEmptyNode(void);                        // Add an empty node
        wxString GetTextStyle (odtTEXT_STYLE style);    // Get text style label
        void FillTableStyles (void);                // Fill XML style nodes with styles that are used in the document
        void FillStyleData (int i, int j, int k);   // Inserts style names for the cells
        void PictureToManifest(wxString &name);     // Adds picture data to the manifest file

        wxDir *m_myDirectory;   // directory for the document
        wxString m_tmpDirPath;  // path for temporary directory
        wxString m_myName;          // document's name
        wxString m_pathSeparator;   // path separator
        bool m_isProcessing;        // is content being written
        bool m_hasTmpDir;   // has temporary directory been created
        short m_error;      // error code
        // Files (XMLs and nodes)
        wxString m_pathToContentXML;
        wxString m_pathToManifestXML;
        wxXmlDocument *m_myContent;     // content.xml pointer
        wxXmlDocument *m_myManifest;    // manifest.xml pointer
        wxXmlNode *m_contentNode;       // text content pointer
        wxXmlNode *m_styleNode;     // styles from content.xml
        wxXmlNode *m_manifestNode;  // formula info etc in manifest
        wxXmlNode *m_lastNode;      // last node for appending text
        // Styles
        wxString m_paragraphStyle;
        wxString m_textStyle;
        wxArrayString m_styleList;  // list of styles using in tables
        // Formulas
        int m_formulaCounter;       // current formula number
        // Tables data
        bool m_doesTableExists;     // is table being processed
        OdtTable *m_tableToAdd;   // current table pointer
        // Object counters
        int m_tableCounter;     // current table number
        int m_pictureCounter;   // current picture number
        wxDateTime m_startTime; // start time of editing a document
};

#endif // WXODTCREATOR_H
