#include "exceptions.hxx"

namespace si {
  exception::exception(char const* message)
      :std::runtime_error{message} { }

  no_such_element_exception::no_such_element_exception(char const* message)
      :exception{message} { }
}
