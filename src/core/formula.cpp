#include "core/formula.h"
#include "common.h"
#include "core/core.h"
#include "core/parser.h"
#include "fonts/alphabet.h"
#include "fonts/fonts.h"
#include "graphic/graphic.h"
#include "res/parser/formula_parser.h"

using namespace std;
using namespace tex;

map<wstring, sptr<TeXFormula>> TeXFormula::_predefinedTeXFormulas;

map<UnicodeBlock, FontInfos*> TeXFormula::_externalFontMap;

float TeXFormula::PIXELS_PER_POINT = 1.f;

void TeXFormula::_init_() {
#ifdef HAVE_LOG
  __dbg("%s\n", "init formula");
#endif  // HAVE_LOG
  // Register external alphabet
  DefaultTeXFont::registerAlphabet(new CyrillicRegistration());
  DefaultTeXFont::registerAlphabet(new GreekRegistration());
#ifdef HAVE_LOG
  __log << "elements in _symbolMappings:" << endl;
  for (auto i : _symbolMappings)
    __log << "\t" << i.first << "->" << i.second << endl;
  __log << "elements in _symbolTextMappings:" << endl;
  for (auto i : _symbolTextMappings)
    __log << "\t" << i.first << "->" << i.second << endl;
  __log << "elements in _symbolFormulaMappings:" << endl;
  for (auto i : _symbolFormulaMappings)
    __log << "\t" << i.first << "->" << i.second << endl;
#endif  // HAVE_LOG
}

TeXFormula::TeXFormula(const TeXParser& tp)
    : _parser(tp.getIsPartial(), L"", this, false) {
  _xmlMap = tp._formula->_xmlMap;
}

TeXFormula::TeXFormula(
    const TeXParser& tp,
    const wstring& s,
    const string& textStyle,
    bool firstpass, bool space)
    : _parser(tp.getIsPartial(), s, this, firstpass, space) {
  _textStyle = textStyle;
  _xmlMap = tp._formula->_xmlMap;
  if (tp.getIsPartial()) {
    try {
      _parser.parse();
    } catch (exception&) {
      if (_root == nullptr) _root = sptr<Atom>(new EmptyAtom());
    }
  } else {
    _parser.parse();
  }
}

TeXFormula::TeXFormula(
    const TeXParser& tp, const wstring& s, const string& textStyle)
    : _parser(tp.getIsPartial(), s, this) {
  _textStyle = textStyle;
  _xmlMap = tp._formula->_xmlMap;
  if (tp.getIsPartial()) {
    try {
      _parser.parse();
    } catch (exception&) {
      if (_root == nullptr) _root = sptr<Atom>(new EmptyAtom());
    }
  } else {
    _parser.parse();
  }
}

TeXFormula::TeXFormula(const TeXParser& tp, const wstring& s, bool firstpass)
    : _parser(tp.getIsPartial(), s, this, firstpass) {
  _textStyle = "";
  _xmlMap = tp._formula->_xmlMap;
  if (tp.getIsPartial()) {
    try {
      _parser.parse();
    } catch (exception&) {}
  } else {
    _parser.parse();
  }
}

TeXFormula::TeXFormula(const TeXParser& tp, const wstring& s)
    : _parser(tp.getIsPartial(), s, this) {
  _textStyle = "";
  _xmlMap = tp._formula->_xmlMap;
  if (tp.getIsPartial()) {
    try {
      _parser.parse();
    } catch (exception&) {
      if (_root == nullptr) _root = sptr<Atom>(new EmptyAtom());
    }
  } else {
    _parser.parse();
  }
}

TeXFormula::TeXFormula() : _parser(L"", this, false) {}

TeXFormula::TeXFormula(const wstring& s) : _parser(s, this) {
  _textStyle = "";
  _parser.parse();
}

TeXFormula::TeXFormula(const wstring& s, bool firstpass)
    : _parser(s, this, firstpass) {
  _textStyle = "";
  _parser.parse();
}

TeXFormula::TeXFormula(const wstring& s, const string& textStyle)
    : _parser(s, this) {
  _textStyle = textStyle;
  _parser.parse();
}

TeXFormula::TeXFormula(
    const wstring& s, const string& textStyle, bool firstpass, bool space)
    : _parser(s, this, firstpass, space) {
  _textStyle = textStyle;
  _parser.parse();
}

TeXFormula::TeXFormula(const TeXFormula* f) {
  if (f != nullptr) addImpl(f);
}

void TeXFormula::setLaTeX(const wstring& latex) {
  _parser.reset(latex);
  if (!latex.empty()) _parser.parse();
}

TeXFormula* TeXFormula::add(const sptr<Atom>& el) {
  if (el == nullptr) return this;
  auto atom = dynamic_pointer_cast<MiddleAtom>(el);
  if (atom != nullptr) _middle.push_back(atom);
  if (_root == nullptr) {
    _root = el;
    return this;
  }
  RowAtom* rm = dynamic_cast<RowAtom*>(_root.get());
  if (rm == nullptr) _root = sptr<Atom>(new RowAtom(_root));
  rm = static_cast<RowAtom*>(_root.get());
  rm->add(el);
  TypedAtom* ta = dynamic_cast<TypedAtom*>(el.get());
  if (ta != nullptr) {
    int rt = ta->getRightType();
    if (rt == TYPE_BINARY_OPERATOR || rt == TYPE_RELATION)
      rm->add(sptr<Atom>(new BreakMarkAtom()));
  }
  return this;
}

TeXFormula* TeXFormula::append(bool isPartial, const wstring& s) {
  if (!s.empty()) {
    TeXParser tp(isPartial, s, this);
    tp.parse();
  }
  return this;
}

TeXFormula* TeXFormula::append(const wstring& s) {
  return append(false, s);
}

void TeXFormula::addImpl(const TeXFormula* f) {
  if (f != nullptr) {
    RowAtom* rm = dynamic_cast<RowAtom*>(f->_root.get());
    if (rm != nullptr)
      add(sptr<Atom>(new RowAtom(f->_root)));
    else
      add(f->_root);
  }
}

sptr<Box> TeXFormula::createBox(_out_ TeXEnvironment& style) {
  if (_root == nullptr) return sptr<Box>(new StrutBox(0, 0, 0, 0));
  return _root->createBox(style);
}

void TeXFormula::setDEBUG(bool b) {
  Box::mDebug = b;
}

TeXFormula* TeXFormula::setBackground(color c) {
  if (istrans(c)) return this;
  ColorAtom* ca = dynamic_cast<ColorAtom*>(_root.get());
  if (ca != nullptr)
    _root = sptr<Atom>(new ColorAtom(c, TRANS, _root));
  else
    _root = sptr<Atom>(new ColorAtom(_root, c, TRANS));
  return this;
}

TeXFormula* TeXFormula::setColor(color c) {
  if (istrans(c)) return this;
  ColorAtom* ca = dynamic_cast<ColorAtom*>(_root.get());
  if (ca != nullptr)
    _root = sptr<Atom>(new ColorAtom(TRANS, c, _root));
  else
    _root = sptr<Atom>(new ColorAtom(_root, TRANS, c));
  return this;
}

TeXFormula* TeXFormula::setFixedTypes(int left, int right) {
  _root = sptr<Atom>(new TypedAtom(left, right, _root));
  return this;
}

sptr<TeXFormula> TeXFormula::get(const wstring& name) {
  auto it = _predefinedTeXFormulas.find(name);
  if (it == _predefinedTeXFormulas.end()) {
    auto i = _predefinedTeXFormulasAsString.find(name);
    if (i == _predefinedTeXFormulasAsString.end())
      throw ex_formula_not_found(wide2utf8(name.c_str()));
    sptr<TeXFormula> tf(new TeXFormula(i->second));
    RowAtom* ra = dynamic_cast<RowAtom*>(tf->_root.get());
    if (ra == nullptr) {
      _predefinedTeXFormulas[name] = tf;
    }
    return tf;
  }
  return it->second;
}

void TeXFormula::setDPITarget(float dpi) {
  PIXELS_PER_POINT = dpi / 72.f;
}

bool TeXFormula::isRegisteredBlock(const UnicodeBlock& block) {
  return _externalFontMap.find(block) != _externalFontMap.end();
}

FontInfos* TeXFormula::getExternalFont(const UnicodeBlock& block) {
  auto it = _externalFontMap.find(block);
  FontInfos* infos = nullptr;
  if (it == _externalFontMap.end()) {
    infos = new FontInfos("SansSerif", "Serif");
    _externalFontMap[block] = infos;
  } else {
    infos = it->second;
  }
  return infos;
}

void TeXFormula::addSymbolMappings(const string& file) {
  TeXFormulaSettingParser parser(file);
  parser.parseSymbol(_symbolMappings, _symbolTextMappings);
  parser.parseSymbol2Formula(_symbolFormulaMappings, _symbolTextMappings);
}

void TeXFormula::_free_() {
  for (auto i : _externalFontMap) delete i.second;
  _externalFontMap.clear();
}

/*************************************** ArrayOfAtoms implementation ******************************/

ArrayOfAtoms::ArrayOfAtoms() : _row(0), _col(0) {
  _array.push_back(vector<sptr<Atom>>());
}

void ArrayOfAtoms::addCol() {
  _array[_row].push_back(_root);
  _root = nullptr;
  _col++;
}

void ArrayOfAtoms::addCol(int n) {
  _array[_row].push_back(_root);
  for (int i = 1; i < n - 1; i++) {
    _array[_row].push_back(nullptr);
  }
  _root = nullptr;
  _col += n;
}

void ArrayOfAtoms::insertAtomIntoCol(int col, const sptr<Atom>& atom) {
  _col++;
  for (size_t j = 0; j < _row; j++) {
    auto it = _array[j].begin();
    _array[j].insert(it + col, atom);
  }
}

void ArrayOfAtoms::addRow() {
  addCol();
  _array.push_back(vector<sptr<Atom>>());
  _row++;
  _col = 0;
}

void ArrayOfAtoms::addRowSpecifier(const sptr<CellSpecifier>& spe) {
  auto it = _rowSpecifiers.find(_row);
  if (it == _rowSpecifiers.end())
    _rowSpecifiers[_row] = vector<sptr<CellSpecifier>>();
  _rowSpecifiers[_row].push_back(spe);
}

void ArrayOfAtoms::addCellSpecifier(const sptr<CellSpecifier>& spe) {
  string str = tostring(_row) + tostring(_col);
  auto it = _cellSpecifiers.find(str);
  if (it == _cellSpecifiers.end())
    _cellSpecifiers[str] = vector<sptr<CellSpecifier>>();
  _cellSpecifiers[str].push_back(spe);
}

int ArrayOfAtoms::rows() const {
  return _row;
}

int ArrayOfAtoms::cols() const {
  return _col;
}

sptr<VRowAtom> ArrayOfAtoms::getAsVRow() {
  VRowAtom* vr = new VRowAtom();
  vr->setAddInterline(true);
  for (size_t i = 0; i < _array.size(); i++) {
    vector<sptr<Atom>>& c = _array[i];
    for (size_t j = 0; j < c.size(); j++) vr->append(c[j]);
  }
  return sptr<VRowAtom>(vr);
}

void ArrayOfAtoms::checkDimensions() {
  if (_array.back().size() != 0 || _root != nullptr) addRow();

  _row = _array.size() - 1;
  _col = _array[0].size();

  // Find the column count of the widest row
  for (size_t i = 1; i < _row; i++) {
    if (_array[i].size() > _col) _col = _array[i].size();
  }

  for (size_t i = 0; i < _row; i++) {
    size_t j = _array[i].size();
    if (j != _col &&
        _array[i][0] != nullptr &&
        _array[i][0]->_type != TYPE_INTERTEXT) {
      // Fill the row with null atom
      vector<sptr<Atom>>& r = _array[i];
      for (; j < _col; j++) r.push_back(nullptr);
    }
  }
}
