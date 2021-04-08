#include "config.h"
#include "..\..\..\vs\resource.h"

#if defined(BUILD_WIN32) && !defined(MEM_CHECK)

#include <Windows.h>
#include <gdiplus.h>
#pragma comment (lib,"Gdiplus.lib")

#include <sstream>
#include <unordered_map>

#include "platform/gdi_win/graphic_win32.h"

using namespace std;
using namespace tex;

/**************************************************************************************************/

int Font_win32::convertStyle(int style) {
  int s;
  switch (style) {
    case PLAIN:
      s = Gdiplus::FontStyleRegular;
      break;
    case BOLD:
      s = Gdiplus::FontStyleBold;
      break;
    case ITALIC:
      s = Gdiplus::FontStyleItalic;
      break;
    case BOLDITALIC:
      s = Gdiplus::FontStyleBoldItalic;
      break;
    default:
      s = -1;
      break;
  }
  return s;
}

const Gdiplus::FontFamily* Font_win32::_serif;
const Gdiplus::FontFamily* Font_win32::_sansserif;

Font_win32::~Font_win32() {
  if (_family == _serif || _family == _sansserif) {
    return;
  }
  delete _family;
}

Font_win32::Font_win32() : _size(0) {}

Font_win32::Font_win32(const string& name, int style, float size) : _size(size) {
  if (_serif == nullptr) {
    _serif = Gdiplus::FontFamily::GenericSerif();
    _sansserif = Gdiplus::FontFamily::GenericSansSerif();
  }
  const Gdiplus::FontFamily* f = nullptr;
  if (name == "Serif" || name == "SansSerif") {
    if (name == "Serif") {
      f = _serif;
    } else {
      f = _sansserif;
    }
  } else {
    wstring wname = utf82wide(name.c_str());
    f = new Gdiplus::FontFamily(wname.c_str());
  }
  int s = convertStyle(style);
  if (!f->IsStyleAvailable(s)) {
    throw ex_invalid_state("specified font style not available!");
  }
  Gdiplus::Font* tf = new Gdiplus::Font(f, _size, s, Gdiplus::UnitPixel);
  _style = s;
  _family = f;
  _typeface = sptr<Gdiplus::Font>(tf);
}

std::unordered_map<std::wstring, int> gFontList = {
	{ L"res/cyrillic/cyrillic.map.xml" , 101 },
	{ L"res/cyrillic/language_cyrillic.xml", 102 },
	{ L"res/cyrillic/mappings_cyrillic.xml", 103 },
	{ L"res/cyrillic/symbols_cyrillic.xml", 104 },
	{ L"res/cyrillic/wnbx10.ttf", 105 },
	{ L"res/cyrillic/wnbx10.xml", 106 },
	{ L"res/cyrillic/wnbxti10.ttf", 107 },
	{ L"res/cyrillic/wnbxti10.xml", 108 },
	{ L"res/cyrillic/wnr10.ttf", 109 },
	{ L"res/cyrillic/wnr10.xml", 110 },
	{ L"res/cyrillic/wnss10.ttf", 111 },
	{ L"res/cyrillic/wnss10.xml", 112 },
	{ L"res/cyrillic/wnssbx10.ttf", 113 },
	{ L"res/cyrillic/wnssbx10.xml", 114 },
	{ L"res/cyrillic/wnssi10.ttf", 115 },
	{ L"res/cyrillic/wnssi10.xml", 116 },
	{ L"res/cyrillic/wnti10.ttf", 117 },
	{ L"res/cyrillic/wnti10.xml", 118 },
	{ L"res/cyrillic/wntt10.ttf", 119 },
	{ L"res/cyrillic/wntt10.xml", 120 },
	{ L"res/fonts/base/cmex10.ttf", 121 },
	{ L"res/fonts/base/cmmi10.ttf", 122 },
	{ L"res/fonts/base/cmmib10.ttf", 123 },
	{ L"res/fonts/euler/eufb10.ttf", 124 },
	{ L"res/fonts/euler/eufm10.ttf", 125 },
	{ L"res/fonts/latin/bi10.ttf", 126 },
	{ L"res/fonts/latin/bx10.ttf", 127 },
	{ L"res/fonts/latin/cmr10.ttf", 128 },
	{ L"res/fonts/latin/i10.ttf", 129 },
	{ L"res/fonts/latin/r10.ttf", 130 },
	{ L"res/fonts/latin/sb10.ttf", 131 },
	{ L"res/fonts/latin/sbi10.ttf", 132 },
	{ L"res/fonts/latin/si10.ttf", 133 },
	{ L"res/fonts/latin/ss10.ttf", 134 },
	{ L"res/fonts/latin/tt10.ttf", 135 },
	{ L"res/fonts/latin/optional/cmbx10.ttf", 136 },
	{ L"res/fonts/latin/optional/cmbxti10.ttf", 137 },
	{ L"res/fonts/latin/optional/cmss10.ttf", 138 },
	{ L"res/fonts/latin/optional/cmssbx10.ttf", 139 },
	{ L"res/fonts/latin/optional/cmssi10.ttf", 140 },
	{ L"res/fonts/latin/optional/cmti10.ttf", 141 },
	{ L"res/fonts/latin/optional/cmtt10.ttf", 142 },
	{ L"res/fonts/maths/cmbsy10.ttf", 143 },
	{ L"res/fonts/maths/cmsy10.ttf", 144 },
	{ L"res/fonts/maths/msam10.ttf", 145 },
	{ L"res/fonts/maths/msbm10.ttf", 146 },
	{ L"res/fonts/maths/rsfs10.ttf", 147 },
	{ L"res/fonts/maths/special.ttf", 148 },
	{ L"res/fonts/maths/stmary10.ttf", 149 },
	{ L"res/fonts/maths/optional/dsrom10.ttf", 150 },
	{ L"res/greek/fcmbipg.ttf", 151 },
	{ L"res/greek/fcmbipg.xml", 152 },
	{ L"res/greek/fcmbpg.ttf", 153 },
	{ L"res/greek/fcmbpg.xml", 154 },
	{ L"res/greek/fcmripg.ttf", 155 },
	{ L"res/greek/fcmripg.xml", 156 },
	{ L"res/greek/fcmrpg.ttf", 157 },
	{ L"res/greek/fcmrpg.xml", 158 },
	{ L"res/greek/fcsbpg.ttf", 159 },
	{ L"res/greek/fcsbpg.xml", 160 },
	{ L"res/greek/fcsropg.ttf", 161 },
	{ L"res/greek/fcsropg.xml", 162 },
	{ L"res/greek/fcsrpg.ttf", 163 },
	{ L"res/greek/fcsrpg.xml", 164 },
	{ L"res/greek/fctrpg.ttf", 165 },
	{ L"res/greek/fctrpg.xml", 166 },
	{ L"res/greek/greek.map.xml", 167 },
	{ L"res/greek/language_greek.xml", 168 },
	{ L"res/greek/mappings_greek.xml", 169 },
	{ L"res/greek/symbols_greek.xml", 170 }
};

Font_win32::Font_win32(const string& file, float size) {
  // add font to font collection
  // some like fontconfig
  Gdiplus::PrivateFontCollection c;
  const wstring wfile = utf82wide(file.c_str());

  const auto lIt = gFontList.find(wfile);
  if (lIt == gFontList.end())
      throw ex_invalid_state("cannot load font file " + file);

  // Searches the given resource
  auto* const lResourceHandle = ::FindResourceA(nullptr, MAKEINTRESOURCEA(lIt->second), "BINARY");
  auto* const lGlobalHandle = ::LoadResource(nullptr, lResourceHandle);

  c.AddMemoryFont(::LockResource(lGlobalHandle), ::SizeofResource(nullptr, lResourceHandle));
	
  //c.AddFontFile(wfile.c_str());
	
  Gdiplus::FontFamily* ff = new Gdiplus::FontFamily();
  int num = 0;
  c.GetFamilies(1, ff, &num);
  if (num <= 0) {
    throw ex_invalid_state("cannot load font file " + file);
  }
  // search order :
  // regular -> bold -> italic -> bold-italic
  _size = size;
  _family = ff;
  Gdiplus::Font* f = nullptr;
  if (ff->IsStyleAvailable(Gdiplus::FontStyleRegular)) {
    f = new Gdiplus::Font(ff, size, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
    _style = Gdiplus::FontStyleRegular;
  } else if (ff->IsStyleAvailable(Gdiplus::FontStyleBold)) {
    f = new Gdiplus::Font(ff, size, Gdiplus::FontStyleBold, Gdiplus::UnitPixel);
    _style = Gdiplus::FontStyleBold;
  } else if (ff->IsStyleAvailable(Gdiplus::FontStyleItalic)) {
    f = new Gdiplus::Font(ff, size, Gdiplus::FontStyleItalic, Gdiplus::UnitPixel);
    _style = Gdiplus::FontStyleItalic;
  } else if (ff->IsStyleAvailable(Gdiplus::FontStyleBoldItalic)) {
    f = new Gdiplus::Font(ff, size, Gdiplus::FontStyleBoldItalic, Gdiplus::UnitPixel);
    _style = Gdiplus::FontStyleBoldItalic;
  } else {
    throw ex_invalid_state("no available font in file " + file);
  }
  _typeface = sptr<Gdiplus::Font>(f);
}

float Font_win32::getSize() const {
  return _size;
}

sptr<Font> Font_win32::deriveFont(int style) const {
  int s = convertStyle(style);
  if (!_family->IsStyleAvailable(s)) {
    throw ex_invalid_state("specified font style not available!");
  }
  Font_win32* f = new Font_win32();
  f->_family = _family;
  f->_style = s;
  f->_size = _size;
  Gdiplus::Font* ff = new Gdiplus::Font(&(*_family), _size, s, Gdiplus::UnitPixel);
  f->_typeface = sptr<Gdiplus::Font>(ff);
  return sptr<Font>(f);
}

bool Font_win32::operator==(const Font& ft) const {
  const Font_win32& f = static_cast<const Font_win32&>(ft);
  return _typeface.get() == f._typeface.get() && _size == f._size;
}

bool Font_win32::operator!=(const Font& f) const {
  return !(*this == f);
}

Font* Font::create(const string& file, float s) {
  return new Font_win32(file, s);
}

sptr<Font> Font::_create(const string& name, int style, float size) {
  return sptr<Font>(new Font_win32(name, style, size));
}

/**************************************************************************************************/

Gdiplus::Graphics* TextLayout_win32::_g = nullptr;
Gdiplus::Bitmap* TextLayout_win32::_img = nullptr;
const Gdiplus::StringFormat* TextLayout_win32::_format = nullptr;

TextLayout_win32::TextLayout_win32(const wstring& src, const sptr<Font_win32>& font)
    : _txt(src), _font(font) {
  if (_img == nullptr) {
    _img = new Gdiplus::Bitmap(1, 1, PixelFormat32bppARGB);
    _g = Gdiplus::Graphics::FromImage(_img);
    _format = Gdiplus::StringFormat::GenericTypographic();
  }
}

void TextLayout_win32::getBounds(_out_ Rect& r) {
  int em = _font->_family->GetEmHeight(_font->_style);
  int ascent = _font->_family->GetCellAscent(_font->_style);
  float ap = _font->getSize() * ascent / em;
  Gdiplus::RectF r1;
  _g->MeasureString(
      _txt.c_str(), wcslen(_txt.c_str()), &(*_font->_typeface), Gdiplus::PointF(0, 0), _format, &r1);
  r.x = 0;
  r.y = -ap;
  r.w = r1.Width;
  r.h = r1.Height;
}

void TextLayout_win32::draw(Graphics2D& g2, float x, float y) {
  const Font* prev = g2.getFont();
  g2.setFont(_font.get());
  g2.drawText(_txt, x, y);
  g2.setFont(prev);
}

sptr<TextLayout> TextLayout::create(const wstring& src, const sptr<Font>& font) {
  sptr<Font_win32> f = static_pointer_cast<Font_win32>(font);
  return sptr<TextLayout>(new TextLayout_win32(src, f));
}

/**************************************************************************************************/

const Gdiplus::StringFormat* Graphics2D_win32::_format = nullptr;
const Font* Graphics2D_win32::_defaultFont = nullptr;

Graphics2D_win32::Graphics2D_win32(Gdiplus::Graphics* g) {
  if (_format == nullptr) {
    _format = Gdiplus::StringFormat::GenericTypographic();
  }
  if (_defaultFont == nullptr) {
    _defaultFont = new Font_win32("Arial", PLAIN, 72.f);
  }
  _sx = _sy = 1.f;
  _color = black;
  _font = _defaultFont;
  _g = g;
  _g->ResetTransform();
  _g->SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
  _g->SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
  _pen = new Gdiplus::Pen(Gdiplus::Color(_color));
  _brush = new Gdiplus::SolidBrush(Gdiplus::Color(_color));
}

Graphics2D_win32::~Graphics2D_win32() {
  delete _pen;
  delete _brush;
}

void Graphics2D_win32::setColor(color color) {
  _color = color;
  _pen->SetColor(Gdiplus::Color(color));
  _brush->SetColor(Gdiplus::Color(color));
}

color Graphics2D_win32::getColor() const {
  return _color;
}

void Graphics2D_win32::setStroke(const Stroke& s) {
  _stroke = s;
  _pen->SetWidth(s.lineWidth);
  Gdiplus::LineCap c;
  switch (s.cap) {
    case CAP_BUTT:
      c = Gdiplus::LineCapFlat;
      break;
    case CAP_ROUND:
      c = Gdiplus::LineCapRound;
      break;
    case CAP_SQUARE:
      c = Gdiplus::LineCapSquare;
      break;
  }
  _pen->SetLineCap(c, c, Gdiplus::DashCapRound);
  Gdiplus::LineJoin j;
  switch (s.join) {
    case JOIN_BEVEL:
      j = Gdiplus::LineJoinBevel;
      break;
    case JOIN_ROUND:
      j = Gdiplus::LineJoinRound;
      break;
    case JOIN_MITER:
      j = Gdiplus::LineJoinMiter;
      break;
  }
  _pen->SetLineJoin(j);
  _pen->SetMiterLimit(s.miterLimit);
}

const Stroke& Graphics2D_win32::getStroke() const {
  return _stroke;
}

void Graphics2D_win32::setStrokeWidth(float w) {
  _stroke.lineWidth = w;
  _pen->SetWidth(w);
}

const Font* Graphics2D_win32::getFont() const {
  return _font;
}

void Graphics2D_win32::setFont(const Font* font) {
  _font = font;
}

void Graphics2D_win32::translate(float dx, float dy) {
  _g->TranslateTransform(dx, dy);
}

void Graphics2D_win32::scale(float sx, float sy) {
  _sx *= sx;
  _sy *= sy;
  _g->ScaleTransform(sx, sy);
}

void Graphics2D_win32::rotate(float angle) {
  _g->RotateTransform(angle / PI * 180);
}

void Graphics2D_win32::rotate(float angle, float px, float py) {
  _g->TranslateTransform(px, py);
  _g->RotateTransform(angle / PI * 180);
  _g->TranslateTransform(-px, -py);
}

void Graphics2D_win32::reset() {
  _g->ResetTransform();
  _sx = _sy = 1.f;
}

float Graphics2D_win32::sx() const {
  return _sx;
}

float Graphics2D_win32::sy() const {
  return _sy;
}

void Graphics2D_win32::drawChar(wchar_t c, float x, float y) {
  wchar_t str[]{c, L'\0'};
  drawText(str, x, y);
}

void Graphics2D_win32::drawText(const wstring& c, float x, float y) {
  const wchar_t* str = c.c_str();
  const Font_win32* f = (const Font_win32*)_font;
  int em = f->_family->GetEmHeight(f->_style);
  int ascent = f->_family->GetCellAscent(f->_style);
  float ap = f->getSize() * ascent / em;
  int len = wcslen(str);
  Gdiplus::RectF r1, r2;
  _g->MeasureString(str, len, &(*f->_typeface), Gdiplus::PointF(0, 0), &r1);
  _g->MeasureString(str, len, &(*f->_typeface), Gdiplus::PointF(0, 0), _format, &r2);
  float off = (r1.Width - r2.Width) / 2.f;
  _g->DrawString(str, len, &(*f->_typeface), Gdiplus::PointF(x - off, y - ap), _brush);
}

void Graphics2D_win32::drawLine(float x1, float y1, float x2, float y2) {
  _g->DrawLine(_pen, x1, y1, x2, y2);
}

void Graphics2D_win32::drawRect(float x, float y, float w, float h) {
  _g->DrawRectangle(_pen, x, y, w, h);
}

void Graphics2D_win32::fillRect(float x, float y, float w, float h) {
  _g->FillRectangle(_brush, x, y, w, h);
}

void Graphics2D_win32::drawRoundRect(float x, float y, float w, float h, float rx, float ry) {
  // not supported
  drawRect(x, y, w, h);
}

void Graphics2D_win32::fillRoundRect(float x, float y, float w, float h, float rx, float ry) {
  // not supported
  fillRect(x, y, w, h);
}

#endif
