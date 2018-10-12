#include "../include/OdtCreator.h"
#include "../include/OdtZipDirTraverser.h"
#include "../include/OdtXmlNodes.h"
#include <wx/zipstrm.h>
#include <wx/wfstream.h>
#include <wx/datstrm.h>
#include <wx/arrstr.h>

// Writes the odt-file:
// Enumerates files and directories in the temporary folder through DirTraverser class.
// Archives the objects into a zip file with odt-extension
bool OdtCreator::Write ()
{
    wxArrayString items;            // list of files
    wxArrayString folders;          // list of folders
    m_error = ODT_ERROR_NO_ERRORS;  // reset errors
    // Is the document being processed
    if (!m_isProcessing)
    {
        m_error = ODT_ERROR_NO_DOC_IN_PROCESSING;
        return false;
    }
    // Add table styles that are used
    FillTableStyles();  // see wxOdtCreatorDriver.cpp
    // Save parts
    m_myContent->Save(m_pathToContentXML);
    m_myManifest->Save(m_pathToManifestXML);
    m_myContent = NULL;
    m_myManifest = NULL;
    ProcessMetaXml();
    // Folder enumeration
    wxZipDirTraverser traverser(items, folders);
    traverser.SetRelativeDir(m_tmpDirPath);
    wxDir dir(m_tmpDirPath);
    dir.Traverse(traverser);
    ClearError();
    // Writes file
    wxFFileOutputStream file_out(m_myDirectory->GetName()<<m_pathSeparator<<m_myName<<wxT(".")<<ODT_EXTENSION);
    wxZipOutputStream zip_out(file_out);
    wxFFileInputStream *file_in;
    wxZipEntry *zip_entry;
    wxFileName *file_name;
    // Folders
    for (unsigned int i=0; i<folders.GetCount(); i++)
    {
        zip_out.PutNextDirEntry(folders.Item(i));
    }
    // Files
    for (unsigned int i=0; i<items.GetCount(); i++)
    {
        file_in = new wxFFileInputStream(items.Item(i));
        file_name = new wxFileName(items.Item(i));
        if (file_name->MakeRelativeTo(m_tmpDirPath))
        {
            zip_entry = new wxZipEntry(file_name->GetFullPath());
            zip_out.PutNextEntry(zip_entry);
            zip_out.Write(*(file_in));
        }
        else
        {
            m_error = ODT_ERROR_CANNOT_BUILD_FILE;
            delete file_in;
            delete file_name;
            return false;
        }
        delete file_in;
        delete file_name;
    }
    zip_out.CloseEntry();
    zip_out.Close();
    file_out.Close();
    this->Close();
    return true;
}


