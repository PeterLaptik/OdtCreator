#include "../include/OdtCreator.h"
#include "../include/OdtXmlNodes.h"

OdtCreator::OdtCreator()
{
    m_myDirectory = new wxDir(wxGetCwd());  // current working directory
    m_myName = ODT_DEFAULT_NAME;            // default file name
    m_isProcessing = false;
    m_hasTmpDir = false;
    m_error = 0;
    m_myContent = NULL;
    m_myManifest = NULL;
    m_manifestNode = NULL;
    m_lastNode = NULL;
    m_doesTableExists = false;
}

OdtCreator::OdtCreator(const wxString &myPath, const wxString &m_myName)
{
    wxFileName myFullPath(myPath);
    m_myDirectory = new wxDir(myFullPath.GetPath());    // custom directory
    this->m_myName = m_myName;                          // custom file name
    m_isProcessing = false;
    m_hasTmpDir = false;
    m_error = 0;
    m_myContent = NULL;
    m_myManifest = NULL;
    m_manifestNode = NULL;
    m_lastNode = NULL;
    m_doesTableExists = false;
}

OdtCreator::OdtCreator(const wxString &fullPath)
{
    wxFileName myFullPath(fullPath);
    m_myDirectory = new wxDir(myFullPath.GetPath());    // custom full path
    m_myName = myFullPath.GetName();
    m_isProcessing = false;
    m_hasTmpDir = false;
    m_error = 0;
    m_myContent = NULL;
    m_myManifest = NULL;
    m_manifestNode = NULL;
    m_lastNode = NULL;
    m_doesTableExists = false;
}

OdtCreator::~OdtCreator()
{
    delete m_myDirectory;
    // Remove temporary directory
    if ((m_hasTmpDir) && (wxDir::Exists(m_tmpDirPath)))
    {
        wxDir::Remove(m_tmpDirPath, wxPATH_RMDIR_RECURSIVE);
    }
    if (m_myContent!=NULL)
    {
        delete m_myContent;
    }
    if (m_myManifest!=NULL)
    {
        delete m_myManifest;
    }
    // Remove not inserted table if exists
    if (m_doesTableExists)
    {
        delete m_tableToAdd;
    }
}

// File close. Abort document creating
// Nothing is being saved to disc
void OdtCreator::Close()
{
    // Clear all
    delete m_myDirectory;
    // Remove temporary directory
    if ((m_hasTmpDir) && (wxDir::Exists(m_tmpDirPath)))
    {
        wxDir::Remove(m_tmpDirPath, wxPATH_RMDIR_RECURSIVE);
    }

    if (m_myContent!=NULL)
    {
        delete m_myContent;
    }

    if (m_myManifest!=NULL)
    {
        delete m_myManifest;
    }

    // Remove not inserted table if exists
    if (m_doesTableExists)
    {
        delete m_tableToAdd;
    }
    // Set variables to initial values
    m_myDirectory = new wxDir(wxGetCwd());    // current working directory
    m_myName = ODT_DEFAULT_NAME;              // default file name
    m_isProcessing = false;
    m_hasTmpDir = false;
    m_error = 0;
    m_myContent = NULL;
    m_myManifest = NULL;
    m_manifestNode = NULL;
    m_lastNode = NULL;
    m_tableToAdd = NULL;
    m_doesTableExists = false;
}

// Sets or changes current path.
// Returns false if the files is being written (isProceccing value).
// Returns true if the path has been changed
bool OdtCreator::SetPath(const wxString &myPath)
{
    wxFileName myFullPath(myPath);
    ClearError();
    if (m_isProcessing)
    {
        m_error = ODT_ERROR_FILE_IS_PROCESSING;
        return false;
    }
    // Change directory
    delete m_myDirectory;
    m_myDirectory = new wxDir(myFullPath.GetPath());
    return true;
}

// Sets or changes the filename
// Returns false if the class is initialized.
// Returns true if the name has been changed
bool OdtCreator::SetName (const wxString &m_myName)
{
    ClearError();
    if (m_isProcessing)
    {
        m_error = ODT_ERROR_FILE_IS_PROCESSING;
        return false;
    }
    this->m_myName = m_myName;
    return true;
}

// Checks if the file path and file name are OK
// Returns true if initialization is OK.
// Returns false if initialization fails.
// After 'Init'-method @Name and @Path cannot be changed: the files are being written
bool OdtCreator::Init()
{
    ClearError();

    if ((m_myDirectory->GetName()==wxEmptyString) || (m_myName==wxEmptyString))
    {
        m_error = ODT_ERROR_WRONG_FILEPATH;
        return false;
    }
    // Separator definition
    m_pathSeparator = wxFileName::GetPathSeparator();
    // Checks whether object has been already initialized
    if (m_isProcessing)
    {
        m_error = ODT_ERROR_FILE_IS_PROCESSING;
        return false;
    }
    // Does file exists?
    if (wxFileName::FileExists(m_myDirectory->GetName() + m_pathSeparator + m_myName + mDot + ODT_EXTENSION))
    {
        m_error = ODT_ERROR_FILE_EXISTS;
        return false;
    }
    // temporary directory
    m_tmpDirPath = m_myDirectory->GetName() + m_pathSeparator + mPrefFileName + m_myName;
    // Does temporary folder exist?
    m_tmpDirPath.Clear();
    m_tmpDirPath = m_myDirectory->GetName()<<m_pathSeparator<<mPrefFileName<<m_myName;
    if (wxDir::Exists(m_tmpDirPath))
    {
        m_error = ODT_ERROR_TMP_FOLDER_EXISTS;
        return false;
    }
    // Create directory
    if (!wxDir::Make(m_tmpDirPath))
    {
        m_error = ODT_ERROR_COULD_NOT_MAKE_DIR;
        return false;
    }
    m_hasTmpDir = true;     // the directory has been created
    m_isProcessing = true;  // the file is being written
    MakeTemplates();        // see wxOdtCreatorTemplates
    InitXmlNodes();
    return true;
}

// Returns number of last error
short OdtCreator::GetError()
{
    return m_error;
}

// Sets error number to 'No errors'
void OdtCreator::ClearError()
{
    m_error = ODT_ERROR_NO_ERRORS;
}
