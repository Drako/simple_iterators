#pragma once

#ifndef SI_EXCEPTIONS_HXX
#define SI_EXCEPTIONS_HXX

#include <stdexcept>

namespace si {
  struct exception : std::runtime_error {
    explicit exception(std::string const& message);
  };

  struct no_such_element_exception : exception {
    explicit no_such_element_exception(std::string const& message);
  };
}

#endif // SI_EXCEPTIONS_HXX
