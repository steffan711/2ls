/*******************************************************************\

Module: Showing Stuff

Author: Daniel Kroening, kroening@kroening.com

\*******************************************************************/

#ifndef CPROVER_SUMMARIZER_SHOW_H
#define CPROVER_SUMMARIZER_SHOW_H

#include <string>
#include <ostream>

#include <solvers/prop/prop_conv.h>

#include <goto-programs/goto_model.h>

class message_handlert;

void show_ssa(
  const goto_modelt &,
  bool simplify,
  std::ostream &,
  message_handlert &);

void show_defs(
  const goto_modelt &,
  std::ostream &,
  message_handlert &);

void show_assignments(
  const goto_modelt &,
  std::ostream &,
  message_handlert &);

void show_guards(
  const goto_modelt &,
  std::ostream &,
  message_handlert &);

void show_fixed_points(
  const goto_modelt &,
  bool simplify,
  std::ostream &,
  message_handlert &);

//shows raw error trace
void show_error_trace(
  const irep_idt &property_id,
  const local_SSAt &SSA, 
  prop_convt &solver,
  std::ostream &,
  message_handlert &);



#endif
