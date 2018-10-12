/***************************************************************
**********  Inserts picture into the document*******************
****************************************************************/

#include <wx/wx.h>
#include <wx/app.h>
#include <wx/cmdline.h>
#include "../../class/include/OdtCreator.h"

int main(int argc, char** argv)
{
    OdtCreator myDoc;

    myDoc.Init();
    myDoc.SetParagraphStyle(odtParaStandardCenter);
    myDoc.AppendText("Here is the inserted picture:");
    myDoc.InsertPicture("testPic.png");

    if (!myDoc.Write())
        wxPrintf("Error!");

    return 0;
}
