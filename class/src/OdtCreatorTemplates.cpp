#include "../include/OdtCreator.h"
#include "../include/OdtTemplateArray.h"
#include "../include/OdtXmlNodes.h"
#include "../include/OdtXmlMeta.h"
#include <wx/textfile.h>
#include <wx/wfstream.h>
#include <wx/msgdlg.h>

// Creates templates files and folders.
// XML-files are being created from byte-arrays.
// see wxOdtTemplateArray.h
void OdtCreator::MakeTemplates()
{
    //wxTextFile myFile;
    wxFile *fileToWrite;
    wxFileOutputStream *output; // stream to output
    wxString outputPath;        // output path

    // writing 'mimetype'
    outputPath = m_tmpDirPath;
    outputPath<<m_pathSeparator;
    outputPath<<ODT_FILENAME_MIME;
    output = new wxFileOutputStream(outputPath);
    output->Write(mimetype, mimetype_size);
    output->Close();
    delete output;

    // writing 'meta.xml'
    // @excluded -- see the method below
    /*
    outputPath = m_tmpDirPath;
    outputPath<<m_pathSeparator;
    outputPath<<ODT_FILENEME_META;
    output = new wxFileOutputStream(outputPath);
    output->Write(meta_xml, meta_xml_size);
    output->Close();
    delete output;
    */

    // writing 'styles.xml'
    outputPath = m_tmpDirPath;
    outputPath<<m_pathSeparator;
    outputPath<<ODT_FILENEME_STYLES;
    output = new wxFileOutputStream(outputPath);
    output->Write(styles_xml, styles_xml_size);
    output->Close();
    delete output;

    // writing 'content.xml'
    outputPath = m_tmpDirPath;
    outputPath<<m_pathSeparator;
    outputPath<<ODT_FILENEME_CONTENT;
    output = new wxFileOutputStream(outputPath);
    output->Write(content_xml, content_xml_size);
    output->Close();
    delete output;

    // writing 'settings.xml'
    outputPath = m_tmpDirPath;
    outputPath<<m_pathSeparator;
    outputPath<<ODT_FILENEME_SETTINGS;
    output = new wxFileOutputStream(outputPath);
    output->Write(settings_xml, settings_xml_size);
    output->Close();
    delete output;

    // Creating subdirectories
    // Thumbnails
    outputPath = m_tmpDirPath;
    outputPath<<m_pathSeparator<<DIR_THUMBNAILS;
    wxDir::Make(outputPath);
    outputPath<<m_pathSeparator;
    outputPath<<ODT_FILENAME_IMAGE;
    output = new wxFileOutputStream(outputPath);
    output->Write(thumbnail_png, thumbnail_png_size);
    output->Close();
    delete output;

    // meta-inf - manifest.xml
    outputPath = m_tmpDirPath;
    outputPath<<m_pathSeparator<<DIR_META_INF;
    wxDir::Make(outputPath);
    outputPath<<m_pathSeparator;
    outputPath<<ODT_FILENAME_MANIFEST;
    output = new wxFileOutputStream(outputPath);
    output->Write(manifest_xml, manifest_xml_size);
    output->Close();
    delete output;

    // Configurations2
    outputPath = m_tmpDirPath;
    outputPath<<m_pathSeparator<<DIR_CONFIGURATIONS2;
    wxDir::Make(outputPath);

    // accelerator
    outputPath = m_tmpDirPath;
    outputPath<<m_pathSeparator;
    outputPath<<DIR_CONFIGURATIONS2<<m_pathSeparator<<DIR_ACCELERATOR;
    wxDir::Make(outputPath);
    outputPath<<m_pathSeparator;
    outputPath<<ODT_FILENAME_CURRENT;
    fileToWrite = new wxFile();
    fileToWrite->Create(outputPath);
    fileToWrite->Write(wxT(""));
    fileToWrite->Close();
    delete fileToWrite;

    // floater
    outputPath = m_tmpDirPath;
    outputPath<<m_pathSeparator;
    outputPath<<DIR_CONFIGURATIONS2<<m_pathSeparator<<DIR_FLOATER;
    wxDir::Make(outputPath);

    // images
    outputPath = m_tmpDirPath;
    outputPath<<m_pathSeparator;
    outputPath<<DIR_CONFIGURATIONS2<<m_pathSeparator<<DIR_IMAGES;
    wxDir::Make(outputPath);
    outputPath = m_tmpDirPath;
    outputPath<<m_pathSeparator;
    outputPath<<DIR_CONFIGURATIONS2<<m_pathSeparator<<DIR_IMAGES;
    outputPath<<DIR_BITMAPS;
    wxDir::Make(outputPath);

    // menubar
    outputPath = m_tmpDirPath;
    outputPath<<m_pathSeparator;
    outputPath<<DIR_CONFIGURATIONS2<<m_pathSeparator<<DIR_MENUBAR;
    wxDir::Make(outputPath);

    // popupmenu
    outputPath = m_tmpDirPath;
    outputPath<<m_pathSeparator;
    outputPath<<DIR_CONFIGURATIONS2<<m_pathSeparator<<DIR_POPUPMENU;
    wxDir::Make(outputPath);

    // progressbar
    outputPath = m_tmpDirPath;
    outputPath<<m_pathSeparator;
    outputPath<<DIR_CONFIGURATIONS2<<m_pathSeparator<<DIR_PROGRESSBAR;
    wxDir::Make(outputPath);

    // statusbar
    outputPath = m_tmpDirPath;
    outputPath<<m_pathSeparator;
    outputPath<<DIR_CONFIGURATIONS2<<m_pathSeparator<<DIR_STATUSBAR;
    wxDir::Make(outputPath);

    // toolbar
    outputPath = m_tmpDirPath;
    outputPath<<m_pathSeparator;
    outputPath<<DIR_CONFIGURATIONS2<<m_pathSeparator<<DIR_TOOLBAR;
    wxDir::Make(outputPath);
}

// Processes meta.xml file
void OdtCreator::ProcessMetaXml()
{
    wxString creationDate;
    wxString duration;
    wxString date;
    wxXmlNode *nodeMeta;
    wxXmlNode *node2Add;
    wxString outputPath;
    wxDateTime endTime = wxDateTime::Now();
    wxXmlDocument *doc = new wxXmlDocument();

    // Start editing time
    creationDate.Clear();
    creationDate<<m_startTime.GetYear();
    creationDate<<"-";
    if (m_startTime.GetMonth()<9)
        creationDate<<"0";
    creationDate<<m_startTime.GetMonth() + 1<<"-";
    if (m_startTime.GetDay()<9)
        creationDate<<"0";
    creationDate<<m_startTime.GetDay()<<"T";
    creationDate<<m_startTime.GetHour()<<":"
                        <<m_startTime.GetMinute()<<":"
                        <<m_startTime.GetSecond()<<"."
                        <<m_startTime.GetMillisecond();

    // End editing time
    date.Clear();
    date<<endTime.GetYear();
    date<<"-";
    if (endTime.GetMonth()<9)
        date<<"0";
    date<<endTime.GetMonth() + 1<<"-";
    if (endTime.GetDay()<9)
        date<<"0";
    date<<endTime.GetDay()<<"T";
    date<<endTime.GetHour()<<":"
                        <<endTime.GetMinute()<<":"
                        <<endTime.GetSecond()<<"."
                        <<endTime.GetMillisecond();

    // Process time
    duration = "PT";
    if (endTime.GetHour() - m_startTime.GetHour()<10)
        duration<<"0";
    duration<<endTime.GetHour() - m_startTime.GetHour()<<"H";
    if (endTime.GetMinute() - m_startTime.GetMinute()<10)
        duration<<"0";
    duration<<endTime.GetMinute() - m_startTime.GetMinute()<<"M";
    if (endTime.GetSecond() - m_startTime.GetSecond()<10)
        duration<<"0";
    duration<<endTime.GetSecond() - m_startTime.GetSecond()<<"S";

    // writing meta.xml
    outputPath = m_tmpDirPath;
    outputPath<<m_pathSeparator;
    outputPath<<ODT_FILENEME_META;

    wxXmlNode *nodeRoot = new wxXmlNode(wxXML_ELEMENT_NODE, NODE_meta_Root, wxEmptyString, -1);
    for (int i = meta_root_array_size-1; i>=0; --i)
    {
        nodeRoot->AddAttribute(meta_root_array[i].tag, meta_root_array[i].value);
    }
    doc->SetRoot(nodeRoot);

    nodeMeta = new wxXmlNode(wxXML_ELEMENT_NODE, NODE_meta_Root, wxEmptyString, -1);

    // meta values
    meta_nodes_array[m_meta_date].value = date;
    meta_nodes_array[m_meta_creation_date].value = creationDate;
    meta_nodes_array[m_meta_editing_duration].value = duration;
    for (int i=0; i<meta_nodes_array_size; ++i)
    {
        node2Add = new wxXmlNode(wxXML_ELEMENT_NODE, meta_nodes_array[i].tag, wxEmptyString, -1);
        if (i!=m_meta_stat)
        {
            node2Add->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, meta_nodes_array[i].value, -1));
        }
        else
        {
            for (int i=0; i<meta_statistic_values_size; ++i)
            {
                node2Add->AddAttribute(meta_statistic_values[i].tag, meta_statistic_values[i].value);
            }
        }
        nodeMeta->AddChild(node2Add);
    }

    for (int i=1; i<=meta_number_of_user_defined; ++i)
    {
        wxString value = VAL_meta_userDefinedVal;
        value<<i;
        node2Add = new wxXmlNode(wxXML_ELEMENT_NODE, NODE_meta_userDefined, wxEmptyString, -1);
        node2Add->AddAttribute(TAG_meta_userDefinedTag, value);
        nodeMeta->AddChild(node2Add);
    }

    nodeRoot->AddChild(nodeMeta);
    doc->Save(outputPath);
}
