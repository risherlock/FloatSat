# List of issues
Following is a simple list with the issues identified during development.
This is intended to provide a reminder for what needs to be done and what
could be done.


# Issue1 : No typename added where needed
Tag: templates, nice-to-have

Complex template types cannot be used atm, e.g.:

```
template<typename T> struct O {
  template<typename X> struct I {};
};
// Here is a typename missing.
struct Using : O<bool>::I<char> {};
```


# Issue2: More template testing needed
Tag: templates, tests, nice-to-have

There are a lot of possible cases and corners that need to be checked
with (heavy) templated code. Currently only the most basic usages are
covered. These should be enough for the expected uses cases, but it
certainly does not cover all of C++ possibilities.


# Issue4: Handle virtual inheritance
Tag: release

Currently virtual inheritance is just ignored, that is the generated code
is the same as with normal inheritance. The first solution is to warn
or filter out classes with virtual inheritance (only at first level, or
deeper?). Another solution might be similar to how structure-tree is working:
Collect all members, and remove duplicates. Problem here are access rights.
A third solution would add a layer of indirection: Have the class methods
only cope with the class and move the base handling to a free function or
another method. Friend access might get neccessary with private or protected
inheritance.


# Issue5: Merge template prefixes of framing records
Tag: release, templates

Merging the template prefixes provides a straightforward way to
handle templated records inside templated records.

```{c++}
template<class X> struct O {
  template<class Y> struct I {
    /* ... */
  };
};
template<class X, class Y> O<X>::I<Y>::method() { /* ... */ }
```


# Issue6: Handle anonymous records and namespaces
Tag: release

Anonymous identities cannot be referenced, thus it is impossible
to define a method that belongs to one of them.


---


# Closed3: Handle bit fields
Tag: wont-fix

Bitfields are currently handled as their underlying type, that is
as full width type. This "works" conceptually, it would just waste space.
BUT probably it won't compile because references to bitfields may not be
allowed.

Bitfields may not be bound to a non const reference. So this won't work
with the current design.
