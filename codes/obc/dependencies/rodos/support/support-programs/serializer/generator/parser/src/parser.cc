#include "parser.hh"
#include <set>
#include <vector>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <clang-c/Index.h>
namespace parser {
  /** @internal Does the cursor(kind) reference a record? */
  static bool
    is_record(CXCursorKind kind) {
      return
        (kind == CXCursor_StructDecl) or
        (kind == CXCursor_ClassDecl) or
        (kind == CXCursor_ClassTemplate) or
        (kind == CXCursor_ClassTemplatePartialSpecialization);
    }

  /** @internal Convert libClang string to std::string */
  static std::string
    to_string(CXString&& str) {
      std::string result(clang_getCString(str));
      clang_disposeString(str);
      return result;
    }

  /** @internal Extract all records found.
   *
   * Used as visitor to clang_visitChildren. Emplaces a record for
   * every cursor that is from the main file, is a declaration, a
   * cursor definition and has record kind (@see is_record) in
   * the provided set.
   * Using a set is important to prevent duplicate entries.
   * */
  static CXChildVisitResult
    extract_records(CXCursor cursor, CXCursor parent, CXClientData cd) {
      auto kind = clang_getCursorKind(cursor);
      if (not (
            clang_Location_isFromMainFile(clang_getCursorLocation(cursor)) and
            clang_isDeclaration(kind) and
            clang_isCursorDefinition(cursor))) {
        return CXChildVisit_Continue;
      }
      auto & accum =
        *static_cast<std::set<Record, Record::CompareByOffset> *>(cd);
      if (is_record(kind)) {
        accum.emplace(Record{cursor});
      }
      return CXChildVisit_Recurse;
    }

  static CXChildVisitResult
    extract_bases(CXCursor cursor, CXCursor parent, CXClientData cd) {
      if (clang_getCursorKind(cursor) == CXCursor_CXXBaseSpecifier) {
        auto & accum = *static_cast<std::vector<Base> *>(cd);
        accum.emplace_back(Base{cursor});
      }
      return CXChildVisit_Continue;
    }

  static CXChildVisitResult
    extract_members(CXCursor cursor, CXCursor parent, CXClientData cd) {
      auto const kind = clang_getCursorKind(cursor);
      if ((kind == CXCursor_FieldDecl) or (kind == CXCursor_CXXMethod)) {
        auto & accum = *static_cast<std::vector<Member> *>(cd);
        accum.emplace_back(Member{cursor});
      }
      return CXChildVisit_Continue;
    }


  std::string Member::name() const {
    return to_string(clang_getCursorSpelling(definition));
  }
  std::string Member::type() const {
    return to_string(clang_getTypeSpelling(clang_getCursorType(definition)));
  }
  Member::Kind Member::kind() const {
    switch (clang_getCursorKind(definition)) {
      case CXCursor_FieldDecl:
        return Kind::Field;
      case CXCursor_CXXMethod:
        return Kind::Function;
      default:
        throw std::logic_error("parser::Member::kind - cursor invalid");
    };
  }

  std::string Base::name() const {
    std::string result = to_string(clang_getCursorDisplayName(cursor));
    std::string const class_prefix = "class ";
    std::string const struct_prefix = "struct ";
    if (result.compare(0, class_prefix.size(), class_prefix) == 0) {
      result = result.substr(class_prefix.size());
    }
    else if (result.compare(0, struct_prefix.size(), struct_prefix) == 0) {
      result = result.substr(struct_prefix.size());
    }
    return result;
  }
  Record Base::record() const {
    CXType canonical_type = clang_getCanonicalType(
        clang_getCursorType(cursor));
    // TODO: Template bases?
    return Record{clang_getTypeDeclaration(canonical_type)};
  }
  bool Base::is_virtual() const {
    return clang_isVirtualBase(cursor);
  }

  std::string Record::template_prefix() const {
    auto start = clang_getRangeStart(clang_getCursorExtent(definition));
    auto unit = clang_Cursor_getTranslationUnit(definition);
    CXToken * tokens;
    unsigned int num_tokens;
    clang_tokenize(
        unit,
        clang_getRange(start, clang_getCursorLocation(definition)),
        &tokens, &num_tokens);
    std::string prefix;
    if (num_tokens < 2) {
      throw std::logic_error("parser::Record::template_prefix - num_tokens");
    }
    for (auto i = 0u; i < num_tokens - 2; ++i) {
      prefix += to_string(clang_getTokenSpelling(unit, tokens[i])) + " ";
    }
    clang_disposeTokens(unit, tokens, num_tokens);
    return prefix;
  }

  std::string Record::name() const {
      return to_string(clang_getCursorDisplayName(definition));
  }
  std::vector<Base> Record::bases() const {
    std::vector<Base> gathered_bases;
    clang_visitChildren(
        definition,
        extract_bases,
        &gathered_bases);
    return gathered_bases;
  }
  std::vector<Member> Record::members() const {
    std::vector<Member> gathered_members;
    clang_visitChildren(
        definition,
        extract_members,
        &gathered_members);
    return gathered_members;
  }
  std::vector<std::string> Record::namespaces() const {
    std::vector<std::string> gathered_namespaces;
    CXCursor parent = definition;
    CXCursorKind kind;
    do {
      parent = clang_getCursorSemanticParent(parent);
      kind = clang_getCursorKind(parent);
      if (kind == CXCursor_Namespace) {
        gathered_namespaces.emplace_back(
            to_string(clang_getCursorSpelling(parent)));
      }
    } while (not clang_isTranslationUnit(kind));
    std::reverse(
        std::begin(gathered_namespaces), std::end(gathered_namespaces));
    return gathered_namespaces;
  }
  std::vector<Record> Record::framing_records() const {
    std::vector<Record> gathered_records;
    CXCursor parent = definition;
    CXCursorKind kind;
    do {
      parent = clang_getCursorSemanticParent(parent);
      kind = clang_getCursorKind(parent);
      if (is_record(kind)) {
        gathered_records.emplace_back(Record{parent});
      }
    } while (not clang_isTranslationUnit(kind));
    std::reverse(std::begin(gathered_records), std::end(gathered_records));
    return gathered_records;
  }
  bool Record::is_pod() const {
    return clang_isPODType(clang_getCursorType(definition));
  }

  bool Record::CompareByOffset::operator()(
      Record const & l, Record const & r) const {
    unsigned int l_offset;
    unsigned int r_offset;
    clang_getFileLocation(
        clang_getCursorLocation(l.definition),
        nullptr, nullptr, nullptr,
        &l_offset);
    clang_getFileLocation(
        clang_getCursorLocation(r.definition),
        nullptr, nullptr, nullptr,
        &r_offset);
    return l_offset < r_offset;
  }

  File::File(std::size_t argc, char const * const * argv) {
    index = clang_createIndex(0, 1);
    unit = clang_parseTranslationUnit(index, 0, argv, argc, 0, 0, 0);
    if (not unit) {
      throw std::runtime_error("parser::File::File - translation unit failed");
    }
  }
  File::File(File && f) : index(f.index), unit(f.unit) {
    f.index = nullptr;
    f.unit = nullptr;
  }
  File & File::operator=(File && f) {
    std::swap(index, f.index);
    std::swap(unit, f.unit);
    return *this;
  }

  File::~File() {
    if (unit && index) {
      clang_disposeTranslationUnit(unit);
      clang_disposeIndex(index);
    }
    else if (unit || index) {
      throw std::logic_error("parser::File::~File - Invariant broken");
    }
  }
  std::set<Record, Record::CompareByOffset> File::records() const {
    std::set<Record, Record::CompareByOffset> gathered_records;
    clang_visitChildren(
        clang_getTranslationUnitCursor(unit),
        extract_records,
        &gathered_records);
    return gathered_records;
  }
  std::string File::name() const {
    return to_string(clang_getTranslationUnitSpelling(unit));
  }
}


