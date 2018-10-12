#include "../include/OdtCreator.h"
#include"../include/OdtXmlNodes.h"
#include <wx/filefn.h>

// Inserts an image-file into the document
// Inserts nothing if the file does not exist
// @inPath - path to file
// @width - picture width
// @height - picture height
void OdtCreator::InsertPicture (const wxString &inPath, int width, int height)
{
    wxXmlNode *textNode;
    wxXmlNode *childNode;
    wxXmlNode *pictureNode;
    wxXmlNode *pictureChildNode;
    wxString sValue;
    wxString pictureName;
    wxString pictureFileName;
    wxString destination;

    if (!m_isProcessing)
    {
        m_error = ODT_ERROR_NO_DOC_IN_PROCESSING;
        return;
    }

    // If a temporary directory does not exist
    if (!wxDir::Exists(m_tmpDirPath))
        return;

    // If the file does not exist
    if (!wxFile::Exists(inPath))
        return;

    pictureName = ODT_PICTURE_PREFIX;
    pictureName<<m_pictureCounter;
    pictureFileName = ODT_PICTURE_PREFIX;
    pictureFileName<<m_pictureCounter;
    pictureFileName<<mExt;
    m_pictureCounter++;
    destination = m_tmpDirPath;
    destination<<m_pathSeparator<<ODT_FOLDER_PICTURES<<m_pathSeparator;

    if (!wxDir::Exists(destination))
        wxDir::Make(destination);

    destination<<pictureFileName;
    wxCopyFile(inPath, destination);

    // Add text node
    textNode = new wxXmlNode(wxXML_ELEMENT_NODE, NODE_text, wxEmptyString, -1);
    textNode->AddAttribute(TAG_textStyleName, VAL_standard);

    // Add node
    childNode = new wxXmlNode(wxXML_ELEMENT_NODE, NODE_text, wxEmptyString, -1);
    childNode->AddAttribute(TAG_textStyleName, m_paragraphStyle);
    pictureNode = new wxXmlNode(wxXML_ELEMENT_NODE, NODE_drawFrame, wxEmptyString, -1);
    pictureNode->AddAttribute(TAG_drawName, pictureName);
    sValue.Clear();
    sValue<<width<<mUnits;
    pictureNode->AddAttribute(TAG_svnWidth, sValue);
    pictureNode->AddAttribute(TAG_drawStyleName, mStyleImage);
    pictureNode->AddAttribute(TAG_drawZindex, VAL_drawZindex);
    sValue.Clear();
    sValue<<height<<mUnits;
    pictureNode->AddAttribute(TAG_svnHeight, sValue);
    pictureNode->AddAttribute(TAG_anchorType, VAL_anchorType);
    pictureChildNode = new wxXmlNode(wxXML_ELEMENT_NODE, NODE_drawImage, wxEmptyString, -1);
    pictureChildNode->AddAttribute(TAG_xlinkActuate, VAL_xlinkActuate);
    pictureChildNode->AddAttribute(TAG_xlinkShow, VAL_xlinkShow);
    pictureChildNode->AddAttribute(TAG_xlinkType, VAL_xlinkType);
    sValue.Clear();
    sValue<<ODT_FOLDER_PICTURES<<mSep<<pictureFileName;
    PictureToManifest(sValue); // Add data to manifest XML-file
    pictureChildNode->AddAttribute(TAG_xlinkHref, sValue);
    pictureChildNode->AddAttribute(TAG_loextMimeType, VAL_loextMimeTypePng);

    // Add nodes
    pictureNode->AddChild(pictureChildNode);
    textNode->AddChild(pictureNode);
    m_contentNode->AddChild(textNode);  // blank line
    m_lastNode = NULL;                  // or null (check later)
}

// Adds a picture data to the manifest XML-file
// @picName - relative path to the image
void OdtCreator::PictureToManifest(wxString &picName)
{
    wxXmlNode *nodeToManifest;
    nodeToManifest = new wxXmlNode(wxXML_ELEMENT_NODE, NODE_manifestFileEntry, wxEmptyString, -1);
    nodeToManifest->AddAttribute(TAG_manifestMediaType, VAL_loextMimeTypePng);
    nodeToManifest->AddAttribute(TAG_manifestFullPath, picName);
    m_manifestNode->AddChild(nodeToManifest);
}
