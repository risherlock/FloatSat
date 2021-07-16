#include <cstdlib>
#include <iostream>
#include <functional>
#include "parser.hh"
#include "filter.hh"

std::function<std::string(std::string const &)> generate_namespace_region(
    parser::Record const & record) {
  std::string prefix;
  std::string suffix;
  for (auto const & n : record.namespaces()) {
    prefix += "namespace " + n + " { ";
    suffix += "} ";
  }
  return [prefix, suffix] (std::string const & content) {
    return prefix + content + suffix;
  };
}

std::string generate_scope(parser::Record const & record) {
  std::string scope;
  for (auto const & f : record.framing_records()) {
    scope += f.name() + "::";
  }
  return scope + record.name() + "::";
}

std::string generate_visit_code(parser::Record const & record) {
  auto const namespaced = generate_namespace_region(record);
  auto const scope = generate_scope(record);
  std::string contents;
  for (auto const & base : record.bases()) {
    contents += "v.visit_base(static_cast<" + base.name() + " &>(*this), " +
      (base.is_virtual() ? "true" : "false") + ");\n";
  }
  for (auto const & member : record.members()) {
    if (member.kind() != parser::Member::Kind::Field) {
      continue;
    }
    contents += "v.visit_member(" + member.name() + ");\n";
  }
  return namespaced(
      "template<typename Visitor> void " + scope + "accept(Visitor & v) {" +
      contents +
      "}\n");
}

std::string generate_serialization_code(parser::Record const & record) {
  // For optimization, see http://stackoverflow.com/a/18899027/1116364
  // Alternatively, use an ostream directly, if performance needs it.
  std::string namespace_prefix;
  std::string namespace_suffix;
  for (auto n : record.namespaces()) {
    namespace_prefix += "namespace " + n + " { ";
    namespace_suffix += "} ";
  }
  std::string frame_prefix;
  for (auto f : record.framing_records()) {
    frame_prefix += f.name() + "::";
  }
  std::string serialize_bases;
  std::string deserialize_bases;
  for (auto base : record.bases()) {
    std::string const name = base.name();
    serialize_bases +=
      "  write += BasicSerializers::serialize(static_cast<" + name + " const &>(*this),write);\n";
    deserialize_bases +=
      "  read += BasicSerializers::deserialize(static_cast<" + name + "&>(*this),read);\n";
  }
  std::string serialize_members;
  std::string deserialize_members;
  for (auto member : record.members()) {
    if (member.kind() != parser::Member::Kind::Field) {
      continue;
    }
    std::string const name = member.name();
    serialize_members += "  write += BasicSerializers::serialize(this->" + name + ",write);\n";
    deserialize_members += "  read += BasicSerializers::deserialize(this->" + name + ",read);\n";
  }
  std::string const name = record.name();
  std::string const template_prefix = record.template_prefix();
  return
    namespace_prefix + "\n" +
    template_prefix +
    "int " + frame_prefix + name +
    "::serialize(char * const buffer) const {\n" +
    "  char * write = buffer;\n" +
    serialize_bases +
    serialize_members +
    "  return (write - buffer);\n" +
    "}\n" +
    template_prefix +
    "int " + frame_prefix + name +
    "::deserialize(char const * const buffer) {\n" +
    "  char const * read = buffer;\n" +
    deserialize_bases +
    deserialize_members +
    "  return (read - buffer);\n" +
    "}\n" +
    namespace_suffix + "\n";
}



bool has_serialization_member_functions(parser::Record const & r) {
//  return true; // TODO
  return
    has_member_function(r, "serialize", "int (char *const) const")
    and
    has_member_function(r, "deserialize", "int (const char *const)");
}



bool flag_present(std::size_t argc, char const * const * argv,
    std::string const & flag) {
  for (std::size_t i = 0u; i < argc; ++i) {
    if (flag.compare(argv[i]) == 0) {
      return true;
    }
  }
  return false;
}


int main(int argc, char ** argv) {
  try {
    // Strip programm name from arguments
    std::size_t const num_arguments = argc - 1;
    char const * const * const arguments = argv + 1;
    // Aquire options / flags from arguments
    bool const use_non_pod =
      flag_present(num_arguments, arguments, "--use-non-pod");
    bool const use_virtual_inheritance =
      flag_present(num_arguments, arguments, "--use-virtual-inheritance");
    // Sanity check of aquired options / flags
    if (not use_non_pod && use_virtual_inheritance) {
      std::cerr <<
        "[W] To enable --use-virtual-inheritance pass --use-non-pod, too!"
        << std::endl;
    }
    auto const file = parser::File(num_arguments, arguments);
    std::cout << "\n\n /*** Generated code DO NOT modify * \n";
    std::cout << "   * use generator/serializer **/\n\n";
    std::cout << "#include \"" << file.name() << "\"\n";
    std::cout << "#include \"basic-serializers.hh\"\n";
    std::cout << "\n\n";

    for (auto const record : file.records()) {
      std::string const name = record.name();
      if (has_serialization_member_functions(record)) {
        if (has_virtual_inheritance(record)) {
          if (use_virtual_inheritance) {
            std::cerr
              << "[W] \"" << name << "\" uses virtual inheritance!"
              << std::endl;
          }
          else {
            std::cerr
              << "[I] \"" << name << "\" skiped (virtual inheritance)."
              << std::endl;
            continue;
          }
        }
        if (not record.is_pod()) {
          if (not use_non_pod) {
            std::cerr
              << "[I] \"" << name << "\" skiped (not POD)."
              << std::endl;
            continue;
          }
          else {
            //std::cerr
            //  << "[W] \"" << name << "\" is not a POD type!" << std::endl;
          }
        }
        std::cout << "/* " << record.name() << " */\n"
          << generate_serialization_code(record);
      }
    }
    return EXIT_SUCCESS;
  }
  catch (...) {
    std::cerr << "expected a single .hh input file" << std::endl;
    return EXIT_FAILURE;
  }
}


