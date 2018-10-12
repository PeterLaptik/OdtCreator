#ifndef WXODTXMLMETA_H_INCLUDED
#define WXODTXMLMETA_H_INCLUDED

// Data for meta.xml file

struct MetaDataStructure
{
    wxString tag;
    wxString value;
};

const int meta_number_of_user_defined = 4;
const wxString NODE_meta_Root = "office:document-meta";
const wxString NODE_meta_userDefined = "meta:user-defined";
const wxString TAG_meta_userDefinedTag = "meta:name";
const wxString VAL_meta_userDefinedVal = "Info ";

const int meta_root_array_size = 6;
MetaDataStructure meta_root_array[meta_root_array_size] = {
        "office:version",                            "1.2",
        "xmlns:ooo",                                 "http://openoffice.org/2004/office",
        "xmlns:meta",                                "urn:oasis:names:tc:opendocument:xmlns:meta:1.0",
        "xmlns:dc",                                  "http://purl.org/dc/elements/1.1/",
        "xmlns:xlink",                               "http://www.w3.org/1999/xlink",
        "xmlns:office",                              "urn:oasis:names:tc:opendocument:xmlns:office:1.0"
};

const int m_meta_date = 5;
const int m_meta_creation_date = 0;
const int m_meta_editing_duration = 1;
const int m_meta_stat = 4;
const int meta_nodes_array_size = 6;
MetaDataStructure meta_nodes_array[meta_nodes_array_size] = {
        "meta:creation-date",        "myCreationDate",
        "meta:editing-duration",     "myEditingDuration",
        "meta:editing-cycles",       "2",
        "meta:generator",            "StarOffice/9$Win32 OpenOffice.org_project/310m6$Build-9391",
        "meta:document-statistic",   "tag_list",
        "dc:date",                   "myDate"
};

const int meta_statistic_values_size = 7;
MetaDataStructure meta_statistic_values[] = {
        "meta:character-count",     "0",
        "meta:word-count",          "0",
        "meta:paragraph-count",     "0",
        "meta:page-count",          "1",
        "meta:object-count",        "0",
        "meta:image-count",         "0",
        "meta:table-count",         "0"
};

#endif // WXODTXMLMETA_H_INCLUDED
