#ifndef SERIALIZER_FILTER_HH_
#define SERIALIZER_FILTER_HH_ 1
#include "parser.hh"
bool has_member_field(
    parser::Record const &,
    std::string,
    std::string);
bool has_member_function(
    parser::Record const &,
    std::string,
    std::string);
bool has_member(
    parser::Record const &,
    std::string const,
    std::string const,
    parser::Member::Kind const);

bool has_virtual_inheritance(parser::Record const &);


#endif
