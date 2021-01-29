#pragma once

#ifndef SI_EXCEPTIONS_HXX
#define SI_EXCEPTIONS_HXX

#include <stdexcept>

namespace si {
  struct exception : std::runtime_error {
    explicit exception(char const* message);
  };

  struct no_such_element_exception : exception {
    explicit no_such_element_exception(char const* message);
  };
}

#endif // SI_EXCEPTIONS_HXX
