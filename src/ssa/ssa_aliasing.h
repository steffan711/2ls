/*******************************************************************\

Module: Aliasing Decision

Author: Daniel Kroening, kroening@kroening.com

\*******************************************************************/

#ifndef CPROVER_SSA_ALIASING_H
#define CPROVER_SSA_ALIASING_H

#include <util/std_expr.h>
#include <util/namespace.h>

bool ssa_may_alias(const exprt &, const exprt &, const namespacet &);
exprt ssa_alias_guard(const exprt &, const exprt &, const namespacet &);
exprt ssa_alias_value(const exprt &, const exprt &, const namespacet &);

#endif
