#include "../include/OdtCreator.h"
#include "../include/OdtTemplateArray.h"
#include "../include/OdtXmlNodes.h"
#include <wx/xml/xml.h>
#include <wx/wfstream.h>

// Inserts formula into the document.
// Does not implement the MathML description generating.
// @content - formula via LO/OO-Math syntax
void OdtCreator::InsertFormula (const wxString &content)
{
    wxString docPath;
    wxString formulaName;
    wxXmlDocument *docFormula;
    wxXmlNode *nodeRoot;
    wxXmlNode *nodeSemantics;
    wxXmlNode *formula;
    wxXmlNode *drawObject;
    wxFile *fileToWrite;
    wxFileOutputStream *output;

    if (!m_isProcessing)
    {
        m_error = ODT_ERROR_NO_DOC_IN_PROCESSING;
        return;
    }

    // Make directory
    formulaName=FORMULA_PREFIX;
    formulaName<<m_formulaCounter;
    docPath = m_tmpDirPath;
    docPath<<m_pathSeparator<<formulaName;
    wxDir::Make(docPath);
    docPath<<m_pathSeparator<<ODT_FILENEME_CONTENT;
    fileToWrite = new wxFile();
    fileToWrite->Create(docPath);
    fileToWrite->Write(wxT(""));
    fileToWrite->Close();
    delete fileToWrite;

    // XML Processing
    docFormula = new wxXmlDocument();
    nodeRoot = new wxXmlNode(wxXML_ELEMENT_NODE, NODE_math, wxEmptyString, -1);
    nodeRoot->AddAttribute(TAG_xmlns, VAL_mathML);
    nodeRoot->AddAttribute(TAG_display, VAL_block);
    docFormula->SetRoot(nodeRoot);
    nodeSemantics = new wxXmlNode(nodeRoot, wxXML_ELEMENT_NODE, NODE_semantics);
    formula = new wxXmlNode(wxXML_ELEMENT_NODE, NODE_annotation, wxEmptyString, -1);
    formula->AddAttribute(TAG_encoding, FORMULA_ATTRIBUTE);
    formula->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, content, -1));
    nodeSemantics->AddChild(formula);
    nodeRoot->AddChild(nodeSemantics);

    // Save formula
    docFormula->Save(docPath);
    delete docFormula;
    docPath = m_tmpDirPath;
    docPath<<m_pathSeparator<<formulaName<<m_pathSeparator<<ODT_FILENEME_SETTINGS;

    //Save content xml
    output = new wxFileOutputStream(docPath);
    output->Write(fsettings_xml, fsettings_xml_size);
    output->Close();
    delete output;

    // Writing link to content
    if (m_lastNode!=NULL)
    {
        formula = new wxXmlNode(wxXML_ELEMENT_NODE, NODE_drawFrame, wxEmptyString, -1);
        formula->AddAttribute(TAG_drawStyleName, mStyleFormula);
        formula->AddAttribute(TAG_drawName, formulaName);
        formula->AddAttribute(TAG_anchorType, VAL_asChar);
        formula->AddAttribute(TAG_drawZindex, wxString::Format(wxT("%i"), m_formulaCounter));
        drawObject = new wxXmlNode(wxXML_ELEMENT_NODE, NODE_drawObject, wxEmptyString, -1);
        drawObject->AddAttribute(TAG_xlinkHref, odtObjPath + formulaName);
        drawObject->AddAttribute(TAG_xlinkType, VAL_xlinkType);
        drawObject->AddAttribute(TAG_xlinkShow, VAL_xlinkShow);
        drawObject->AddAttribute(TAG_xlinkActuate, VAL_xlinkActuate);
        formula->AddChild(drawObject);
        m_lastNode->AddChild(formula);
        FormulaToManifest(formulaName);
    }
    else
    {
        AddEmptyNode();
        formula = new wxXmlNode(wxXML_ELEMENT_NODE, NODE_drawFrame, wxEmptyString, -1);
        formula->AddAttribute(TAG_drawStyleName, mStyleFormula);
        formula->AddAttribute(TAG_drawName, formulaName);
        formula->AddAttribute(TAG_anchorType, VAL_asChar);
        formula->AddAttribute(TAG_drawZindex, wxString::Format(wxT("%i"), m_formulaCounter));
        drawObject = new wxXmlNode(wxXML_ELEMENT_NODE, NODE_drawObject, wxEmptyString, -1);
        drawObject->AddAttribute(TAG_xlinkHref, odtObjPath + formulaName);
        drawObject->AddAttribute(TAG_xlinkType, VAL_xlinkType);
        drawObject->AddAttribute(TAG_xlinkShow, VAL_xlinkShow);
        drawObject->AddAttribute(TAG_xlinkActuate, VAL_xlinkActuate);
        formula->AddChild(drawObject);
        m_lastNode->AddChild(formula);
        FormulaToManifest(formulaName);
    }
    m_formulaCounter++;
}

// Inserts formula into a table cell.
// Does not implement the MathML description generating.
// @content - formula via LO/OO-Math syntax
// @row and @column - cell position
void OdtCreator::TableCellInsertFormula (int row, int column,
                                           const wxString &content, int style)
{
    wxString styleName;
    wxString docPath;
    wxString formulaName;
    wxXmlDocument *docFormula;
    wxXmlNode *nodeRoot;
    wxXmlNode *nodeSemantics;
    wxXmlNode *formula;
    wxXmlNode *drawObject;
    wxFile *fileToWrite;
    wxFileOutputStream *output;

    if (!m_isProcessing)
    {
        m_error = ODT_ERROR_NO_DOC_IN_PROCESSING;
        return;
    }

    if (!m_doesTableExists)
        return;

    styleName.Clear();
    styleName<<ODT_TABLE_PREFIX<<style;

    if (style!=odtNO_STYLE)
    {
        if (m_styleList.Index(styleName)==wxNOT_FOUND)
            m_styleList.Add(styleName);
    }

    // Make directory
    formulaName=FORMULA_PREFIX;
    formulaName<<m_formulaCounter;
    docPath = m_tmpDirPath;
    docPath<<m_pathSeparator<<formulaName;
    wxDir::Make(docPath);
    docPath<<m_pathSeparator<<ODT_FILENEME_CONTENT;
    fileToWrite = new wxFile();
    fileToWrite->Create(docPath);
    fileToWrite->Write(wxT(""));
    fileToWrite->Close();
    delete fileToWrite;

    // XML Processing
    docFormula = new wxXmlDocument();
    nodeRoot = new wxXmlNode(wxXML_ELEMENT_NODE, NODE_math, wxEmptyString, -1);
    nodeRoot->AddAttribute(TAG_xmlns, VAL_mathML);
    nodeRoot->AddAttribute(TAG_display, VAL_block);
    docFormula->SetRoot(nodeRoot);
    nodeSemantics = new wxXmlNode(nodeRoot, wxXML_ELEMENT_NODE, NODE_semantics);
    formula = new wxXmlNode(wxXML_ELEMENT_NODE, NODE_annotation, wxEmptyString, -1);
    formula->AddAttribute(TAG_encoding, FORMULA_ATTRIBUTE);
    formula->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, content, -1));
    nodeSemantics->AddChild(formula);
    nodeRoot->AddChild(nodeSemantics);

    // Save formula
    docFormula->Save(docPath);
    delete docFormula;
    docPath = m_tmpDirPath;
    docPath<<m_pathSeparator<<formulaName<<m_pathSeparator<<ODT_FILENEME_SETTINGS;

    //Save content xml
    output = new wxFileOutputStream(docPath);
    output->Write(fsettings_xml, fsettings_xml_size);
    output->Close();
    delete output;

    // Writing link to content
    formula = new wxXmlNode(wxXML_ELEMENT_NODE, NODE_drawFrame, wxEmptyString, -1);
    formula->AddAttribute(TAG_drawStyleName, mStyleFormula);
    formula->AddAttribute(TAG_drawName, formulaName);
    formula->AddAttribute(TAG_anchorType, VAL_asChar);
    formula->AddAttribute(TAG_drawZindex, wxString::Format(wxT("%i"), m_formulaCounter));
    drawObject = new wxXmlNode(wxXML_ELEMENT_NODE, NODE_drawObject, wxEmptyString, -1);
    drawObject->AddAttribute(TAG_xlinkHref, odtObjPath + formulaName);
    drawObject->AddAttribute(TAG_xlinkType, VAL_xlinkType);
    drawObject->AddAttribute(TAG_xlinkShow, VAL_xlinkShow);
    drawObject->AddAttribute(TAG_xlinkActuate, VAL_xlinkActuate);
    formula->AddChild(drawObject);
    FormulaToManifest(formulaName);
    m_tableToAdd->AddCellFormula(row, column, formula, styleName);
    m_formulaCounter++;
}

// Adds a formula information to the manifest-file
// @name - formula name
void OdtCreator::FormulaToManifest(const wxString &name)
{
    wxXmlNode *nodeToManifest;
    nodeToManifest = new wxXmlNode(wxXML_ELEMENT_NODE, NODE_manifestFileEntry, wxEmptyString, -1);
    nodeToManifest->AddAttribute(TAG_manifestMediaType, VAL_loextMimeType);
    nodeToManifest->AddAttribute(TAG_manifestFullPath,
                                 name + *(odtObjPath+1) + ODT_FILENEME_CONTENT);
    m_manifestNode->AddChild(nodeToManifest);
    nodeToManifest = new wxXmlNode(wxXML_ELEMENT_NODE, NODE_manifestFileEntry, wxEmptyString, -1);
    nodeToManifest->AddAttribute(TAG_manifestMediaType, VAL_loextMimeType);
    nodeToManifest->AddAttribute(TAG_manifestFullPath,
                                 name + *(odtObjPath+1) + ODT_FILENEME_SETTINGS);
    m_manifestNode->AddChild(nodeToManifest);
    nodeToManifest = new wxXmlNode(wxXML_ELEMENT_NODE, NODE_manifestFileEntry, wxEmptyString, -1);
    nodeToManifest->AddAttribute(TAG_manifestMediaType, VAL_appName);
    nodeToManifest->AddAttribute(TAG_manifestFullPath, name + *(odtObjPath+1));
    nodeToManifest->AddAttribute(TAG_manifestVersion, mVersion);
    m_manifestNode->AddChild(nodeToManifest);
}

