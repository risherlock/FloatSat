#include "parser.hh"
#include "filter.hh"

bool has_virtual_inheritance(parser::Record const & record) {
  for (auto base : record.bases()) {
    if (base.is_virtual()) {
      return true;
    }
  }
  return false;
}

bool has_member_function(parser::Record const & record,
    std::string name, std::string type) {
  return has_member(record, name, type, parser::Member::Kind::Function);
}
bool has_member_field(parser::Record const & record,
    std::string name, std::string type) {
  return has_member(record, name, type, parser::Member::Kind::Field);
}
bool has_member(parser::Record const & record,
    std::string const name,
    std::string const type,
    parser::Member::Kind const kind) {
  for (auto member : record.members()) {
    if (member.kind() == kind
        and
        member.name() == name
        and
        member.type() == type) {
      return true;
    }
  }
  return false;
}

