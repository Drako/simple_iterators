#include "exceptions.hxx"

namespace si {
  exception::exception(const std::string& message)
      :std::runtime_error{message} { }

  no_such_element_exception::no_such_element_exception(std::string const& message)
      :exception{message} { }
}
