/***************************************************************
*****************  Creates and fills a table  ******************
****************************************************************/

#include <wx/wx.h>
#include <wx/app.h>
#include <wx/cmdline.h>
#include "../../class/include/OdtCreator.h"

int main(int argc, char **argv)
{
    OdtCreator myDoc;
    myDoc.Init();
    myDoc.SetParagraphStyle(odtParaStandardCenter);
    myDoc.AppendText("Here is the table:");
    myDoc.Indent();

    // Table creating
    myDoc.TableCreate(wxT("myTable"), 10, 10);
    for (int i=1; i<10; ++i)
    {
        for (int j=1; j<10; ++j)
        {
            wxString value = wxString::Format("%i-%i", i, j);
            myDoc.TableAddCellValue(i, j, value, odtCellAlignCenter);
        }
    }

    for (int i=1; i<10; ++i)
    {
        wxString value = wxString::Format("a_%i", i);
        myDoc.TableCellInsertFormula(i, 0, value);
    }

    for (int i=1; i<10; ++i)
    {
        wxString value = wxString::Format("b_%i", i);
        myDoc.TableCellInsertFormula(0, i, value, odtCellAlignCenter);
    }

    myDoc.TableAddCellValue(0, 0, "Values", odtCellAlignCenter|odtCellColorRed|odtCellFontBold);

    myDoc.InsertTable();

    if (!myDoc.Write())
        wxPrintf("Error!");

    return 0;
}
