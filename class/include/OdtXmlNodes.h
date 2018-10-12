#ifndef WXODTXMLNODES_H_INCLUDED
#define WXODTXMLNODES_H_INCLUDED

// The header contains:
// Nodes, tags and values for the xml-files that have to be created
// Internal file paths and file names
// Other constants

// XML nodes
const char* const NODE_annotation = "annotation";
const char* const NODE_drawFrame = "draw:frame";
const char* const NODE_drawImage = "draw:image";
const char* const NODE_drawObject = "draw:object";
const char* const NODE_manifestFileEntry = "manifest:file-entry";
const char* const NODE_math = "math";
const char* const NODE_officeAutoStyles = "office:automatic-styles";
const char* const NODE_officeBody = "office:body";
const char* const NODE_officeText = "office:text";
const char* const NODE_semantics = "semantics";
const char* const NODE_styleParagraphProperties = "style:paragraph-properties";
const char* const NODE_styleStyle = "style:style";
const char* const NODE_styleTableProps = "style:table-properties";
const char* const NODE_styleTextProperties = "style:text-properties";
const char* const NODE_tableCell = "table:table-cell";
const char* const NODE_tabCelProps = "style:table-cell-properties";
const char* const NODE_tabColProps = "style:table-column-properties";
const char* const NODE_tableColumn = "table:table-column";
const char* const NODE_tableName = "table:table";
const char* const NODE_text = "text:p";
const char* const NODE_textSpan = "text:span";

// XML tags
const char* const TAG_anchorType = "text:anchor-type";
const char* const TAG_display = "display";
const char* const TAG_drawName = "draw:name";
const char* const TAG_drawStyleName = "draw:style-name";
const char* const TAG_drawZindex = "draw:z-index";
const char* const TAG_encoding = "encoding";
const char* const TAG_fooBorder = "fo:border";
const char* const TAG_foColor = "fo:color";
const char* const TAG_foFontWeight = "fo:font-weight";
const char* const TAG_fooPadding = "fo:padding";
const char* const TAG_foTextAlign = "fo:text-align";
const char* const TAG_loextMimeType = "loext:mime-type";
const char* const TAG_manifestFullPath = "manifest:full-path";
const char* const TAG_manifestMediaType = "manifest:media-type";
const char* const TAG_manifestVersion = "manifest:version";
const char* const TAG_officeParagraphRSID = "officeooo:paragraph-rsid";
const char* const TAG_parentStyleName = "style:parent-style-name";
const char* const TAG_rsid = "officeooo:rsid";
const char* const TAG_styleColumnWidth = "style:column-width";
const char* const TAG_styleFamily = "style:family";
const char* const TAG_styleFontStyleAsian = "style:font-style-asian";
const char* const TAG_styleFontStyleComplex = "style:font-style-complex";
const char* const TAG_styleJustifySingleWord = "style:justify-single-word";
const char* const TAG_styleName = "style:name";
const char* const TAG_styleStyle = "style:style";
const char* const TAG_styleWidth = "style:width";
const char* const TAG_svnHeight = "svg:height";
const char* const TAG_svnWidth = "svg:width";
const char* const TAG_tableAlign = "table:align";
const char* const TAG_tableColSpan = "table:number-columns-spanned";
const char* const TAG_tableFont = "Table_20_Contents";
const char* const TAG_tableName = "table:name";
const char* const TAG_tableRow = "table:table-row";
const char* const TAG_tableRowSpan = "table:number-rows-spanned";
const char* const TAG_tableStyleName = "table:style-name";
const char* const TAG_textNoStyle = "no_style";
const char* const TAG_textP = "text:p";
const char* const TAG_textStyleName = "text:style-name";
const char* const TAG_valueType = "office:value-type";
const char* const TAG_xlinkActuate = "xlink:actuate";
const char* const TAG_xlinkHref = "xlink:href";
const char* const TAG_xlinkShow = "xlink:show";
const char* const TAG_xlinkType = "xlink:type";
const char* const TAG_xmlns = "xmlns";

// XML values
const char* const VAL_anchorType = "paragraph";
const char* const VAL_appName = "application/vnd.oasis.opendocument.formula";
const char* const VAL_asChar = "as-char";
const char* const VAL_block ="block";
const char* const VAL_bold = "bold";
const char* const VAL_borderVal = "0.05pt solid #000000";
const char* const VAL_drawZindex = "1";
const char* const VAL_false = "false";
const char* const VAL_italic = "italic";
const char* const VAL_loextMimeType = "text/xml";
const char* const VAL_loextMimeTypePng = "image/png";
const char* const VAL_mathML = "http://www.w3.org/1998/Math/MathML";
const char* const VAL_paragraph = "paragraph";
const char* const VAL_redColor = "#ff3333";
const char* const VAL_standard = "Standard";
const char* const VAL_string = "string";
const char* const VAL_table = "table";
const char* const VAL_table20contents = "Table_20_Contents";
const char* const VAL_tableCell = "table-cell";
const char* const VAL_tableColumn = "table-column";
const char* const VAL_textP = "text:p";
const char* const VAL_xlinkActuate = "onLoad";
const char* const VAL_xlinkShow = "embed";
const char* const VAL_xlinkType = "simple";

// Folder names
const char* const DIR_THUMBNAILS = "Thumbnails";
const char* const DIR_META_INF = "META-INF";
const char* const DIR_CONFIGURATIONS2 = "Configurations2";
const char* const DIR_ACCELERATOR = "accelerator";
const char* const DIR_FLOATER = "floater";
const char* const DIR_IMAGES = "images";
const char* const DIR_BITMAPS = "Bitmaps";
const char* const DIR_MENUBAR = "menubar";
const char* const DIR_POPUPMENU = "popupmenu";
const char* const DIR_PROGRESSBAR = "progressbar";
const char* const DIR_STATUSBAR = "statusbar";
const char* const DIR_TOOLBAR = "toolbar";
const char* const ODT_FOLDER_PICTURES = "Pictures";

// Filenames
const char* const ODT_EXTENSION = "odt";                    // file extension
const char* const ODT_FILENEME_CONTENT = "content.xml";
const char* const ODT_FILENEME_SETTINGS = "settings.xml";
const char* const ODT_FILENAME_MANIFEST = "manifest.xml";
const char* const ODT_FILENAME_MIME = "mimetype";
const char* const ODT_FILENEME_META = "meta.xml";
const char* const ODT_FILENEME_STYLES = "styles.xml";
const char* const ODT_FILENAME_IMAGE = "thumbnail.png";
const char* const ODT_FILENAME_CURRENT = "current.xml";

// File properties
const char* const ODT_DEFAULT_NAME = "myDoc";       // file name by default
const char* const RSID = "00127062";
const char* const mPrefFileName = "tmp";

// Formula properties
const char* const FORMULA_PREFIX = "Formula_";
const char* const odtObjPath = "./";                // path to internal objects
//const char* const ODT_DEFAULT_SEPARATOR = "\\";     // variable is not used. See m_pathSeparator
const char* const FORMULA_ATTRIBUTE = "StarMath 5.0";
const char* const mStyleFormula = "fr1";
const char* const mVersion = "1.2";

// Image properties
const char* const ODT_PICTURE_PREFIX = "Picture_";
const char* const mUnits = "cm";
const char* const mStyleImage = "fr2";
const char* const mExt = ".png";
const char* const mSep = "/";

// Table properties
const char* const ODT_TABLE_PREFIX = "P";
const char* const wxodt_TABLE_PREFIX = "Table_";
const char* const tableDefaultStyle = "p1";
const char* const mDot = ".";
const char* const mMargVal = "0.097cm";
// dimensions
const short odt_ALIGN_DEFAULT = 0;
const short odt_WIDTH_DEFAULT = 17;

#endif // WXODTXMLNODES_H_INCLUDED
