#include "simplification.h"

Expression * Simplification::simplify(Expression * expression) const {
  Expression * matches[255]; //FIXME: The sized ca be given by our compiler
  if (m_selector->match(expression, matches)) {
    return m_builder->build(matches);
  } else {
    return nullptr;
  }
}
