#ifndef SERIALIZER_PARSER_HH_
#define SERIALIZER_PARSER_HH_ 1
#include <set>
#include <vector>
#include <string>
#include <clang-c/Index.h>

/** Parser logic is packed into 'parser' namespace, wrapping
 * needed libclang Funktions.
 * */
namespace parser {
  /** Represents a member of a structure or class.
   *
   * Objects of this class represent members - both fields and member
   * functions - of a specific parser::Record. Basic properties like
   * name and type spelling may be queried through this class.
   *
   * @warning Instances may become invalid when the associated
   * parser::File object gets invalidated!
   * */
  struct Member {
    /** Enumeration of member kinds.
     * */
    enum class Kind {
      Field, Function
    };

    CXCursor const definition;

    /** Gets the name of the member as specified in C++ source code.
     * */
    std::string name(void) const;

    /** Returns a textual representation of the type of this member.
     *
     * @todo Does it make sense to use the canonical type?
     * */
    std::string type(void) const;

    /** Returns the kind of this member.
     * */
    Kind kind(void) const;
  };

  struct Record;
  /** Represents a base specification of a specific parser::Record.
   * */
  struct Base {
    CXCursor const cursor;

    /** Returns the name of the base class, as specified in the C++
     * source code.
     *
     * @todo Does not reflect 'typename' in base type specifikations.
     * */
    std::string name(void) const;
    Record record(void) const; // TODO remove?

    /** Returns whether this base specification is for a virtual base.
     *
     * Note that this is not transitive: Even if the specified base has
     * a virtual base, it won't show up as virtual here.
     * */
    bool is_virtual(void) const;
  };

  /** Represents a structure or class from the C++ source code.
   *
   * Objects of this class represent structures and classes found by the
   * parser in the C++ source code of the main file. They allow querying
   * for various properties of the structure / class.
   *
   * @warning Instances may get invalid when the associated parser::File
   * gets invalidated!
   * */
  struct Record {
    CXCursor definition; // TODO had to remove const, so make protected

    /** Get a textual representation of the template specifikation.
     *
     * @todo Improve the return type to allow template merging.
     * */
    std::string template_prefix(void) const;

    /** Returns the name of the record, including evtl. template
     * arguments.
     * */
    std::string name(void) const;

    /** Returns the base specifiers of the record, which may be empty.
     * */
    std::vector<Base> bases(void) const;

    /** Returns the members of this record, including member functions.
     * */
    std::vector<Member> members(void) const;

    /** Returns namespaces this record is defined in.
     *
     * @todo Test and refine handling of anonymous namespaces.
     * */
    std::vector<std::string> namespaces(void) const;

    /** Returns the records whose definition 'frame' the definition of
     * this record.
     * */
    std::vector<Record> framing_records(void) const;

    /** Check whether this record is a POD (plain old datatype).
     * */
    bool is_pod(void) const;

    /** Comparator to order records by the file offset of their
     * definitions.
     * */
    struct CompareByOffset {
      /** Check whether one record was defined before another.
       * */
      bool operator()(Record const &, Record const &) const;
    };
  };

  /** Represents the main file that got parsed.
   *
   * Objects of this class contain the state required to perform
   * queries about the contained records etc.
   *
   * @warning Invalidating an object of this class may invalidate all
   * associated objects of classes such as parser::Record, ...
   * */
  class File {
    protected:
    CXIndex  index;
    CXTranslationUnit unit;
    public:
    /** Parse a file, using the given arguments.
     * */
    File(std::size_t, char const * const *);
    File(File const &) = delete;
    File(File &&);
    File & operator=(File const &) = delete;
    File & operator=(File &&);
    ~File();

    /** Get the set of contained records.
     * */
    std::set<Record, Record::CompareByOffset> records(void) const;

    /** Get the filename as specified in the arguments during parsing.
     * */
    std::string name(void) const;
  };
}
#endif
