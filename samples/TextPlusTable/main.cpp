/***************************************************************
**********  Creates text with different styles  ****************
**********        and table with formula        ****************
****************************************************************/

#include <wx/wx.h>
#include <wx/app.h>
#include <wx/cmdline.h>
#include "../../class/include/OdtCreator.h"

int main(int argc, char **argv)
{
    OdtCreator myDoc;
    myDoc.Init();
    myDoc.SetParagraphStyle(odtParaHeader1);
    myDoc.AddText("Header");
    myDoc.SetParagraphStyle(odtParaStandard);
    myDoc.AddText("Simple text.");
    myDoc.AddText("Different", odtTextBold);
    myDoc.AppendText(" text", odtTextItalic);
    myDoc.AppendText(" styles", odtTextBoldItalic);
    myDoc.AppendText(" demo", odtTextRed);
    myDoc.Indent();
    myDoc.AppendText("A table:");

    // Table creating
    myDoc.TableCreate(wxT("myTable"), 3, 5);
    myDoc.TableSetColumnWidth(0,2);
    myDoc.TableSetColumnWidth(1,5);
    myDoc.TableSetColumnWidth(2,8);
    myDoc.TableAddCellValue(0,2,wxT("Hello world"));
    myDoc.TableAddCellValue(0,0, wxT("Zero"), odtCellAlignCenter|odtCellColorRed|odtCellFontBold);
    myDoc.TableSpanRows(3,2,2);
    myDoc.TableAddCellValue(3,2,wxT("United cells"));
    myDoc.TableSpanColumns(2,1,2);
    myDoc.TableAddCellValue(2,1,wxT("United cells"));
    myDoc.TableCellInsertFormula(1,1,"x^2+y^2");
    myDoc.TableSetBorders(true); // Sets visible or invisible borders
    myDoc.InsertTable();

    if (!myDoc.Write())
        wxPrintf("Error!");

    return 0;
}
