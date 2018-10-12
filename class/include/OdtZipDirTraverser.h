#ifndef ZIPTRAVERSER_H_INCLUDED
#define ZIPTRAVERSER_H_INCLUDED

#include <wx/string.h>
#include <wx/arrstr.h>

// Recursive enumeration files and folders
// The class is used for wxZip file set

const int PATH_CHAR_INCREMENTOR = 1;

class wxZipDirTraverser : public wxDirTraverser
{
public:
    wxZipDirTraverser (wxArrayString &items, wxArrayString &folders) :
        myItems(items),
        myFolders(folders)
    {

    }

    virtual wxDirTraverseResult OnFile (const wxString &filename)
    {
        myItems.Add(filename);
        return wxDIR_CONTINUE;
    }

    virtual wxDirTraverseResult OnDir (const wxString &dirname)
    {
        myFolders.Add(dirname.SubString(prefLength, dirname.length()));
        return wxDIR_CONTINUE;
    }

    virtual void SetRelativeDir (const wxString &rootPath)
    {
        prefLength = rootPath.length() + PATH_CHAR_INCREMENTOR;
    }

protected:

private:
    wxArrayString &myItems;     // array of items
    wxArrayString &myFolders;   // array of folders
    int prefLength;             // number of chars in the path. A tail is to be excluded
};

#endif // ZIPTRAVERSER_H_INCLUDED
