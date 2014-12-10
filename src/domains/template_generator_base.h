/*******************************************************************\

Module: Template Generator Base Class

Author: Peter Schrammel

\*******************************************************************/

#ifndef DELTACHECK_TEMPLATE_GENERATOR_BASE_H
#define DELTACHECK_TEMPLATE_GENERATOR_BASE_H

#include <util/options.h>
#include <util/replace_expr.h>

#include "../ssa/local_ssa.h"
#include "../ssa/ssa_unwinder.h"
#include "strategy_solver_base.h"

class template_generator_baset : public messaget
{
public:
  typedef strategy_solver_baset::var_listt var_listt;

  explicit template_generator_baset(optionst &_options,
                                    ssa_local_unwindert &_ssa_local_unwinder)
    : 
  options(_options),
  ssa_local_unwinder(_ssa_local_unwinder)
  {
  }  

  virtual ~template_generator_baset() 
  {
    if(domain_ptr!=NULL) delete domain_ptr;
  }

  virtual void operator()(unsigned _domain_number, 
			  const local_SSAt &SSA, bool forward=true) 
  { 
    domain_number = _domain_number;
    assert(false);
  }

  virtual domaint::var_sett all_vars();

  domaint *domain() { return domain_ptr; }

  domaint::var_specst var_specs;
  replace_mapt renaming_map;
  unsigned domain_number; //serves as id for variables names

  optionst options; // we may override options

protected:
  const ssa_local_unwindert &ssa_local_unwinder;
  domaint* domain_ptr;

  virtual void collect_variables_loop(const local_SSAt &SSA,
                         bool forward);

  void filter_template_domain();
  void filter_equality_domain();

  void add_var(const domaint::vart &var_to_add, 			    
	       const domaint::guardt &pre_guard, 
	       const domaint::guardt &post_guard,
	       const domaint::kindt &kind,
	       domaint::var_specst &var_specs);
  void add_vars(const var_listt &vars_to_add, 			    
		const domaint::guardt &pre_guard, 
		const domaint::guardt &post_guard,
		const domaint::kindt &kind,
		domaint::var_specst &var_specs);
  void add_vars(const local_SSAt::var_listt &vars_to_add,
		const domaint::guardt &pre_guard, 
		const domaint::guardt &post_guard,
		const domaint::kindt &kind,
		domaint::var_specst &var_specs);
  void add_vars(const local_SSAt::var_sett &vars_to_add,
		const domaint::guardt &pre_guard, 
		const domaint::guardt &post_guard,
		const domaint::kindt &kind,
		domaint::var_specst &var_specs);

  void get_pre_post_guards(const local_SSAt &SSA,
			   local_SSAt::nodest::const_iterator n_it,
			   exprt &pre_guard, exprt &post_guard);
  void get_pre_var(const local_SSAt &SSA,
  		   local_SSAt::objectst::const_iterator o_it,
		   local_SSAt::nodest::const_iterator n_it,
		   symbol_exprt &pre_var);

  bool replace_post(replace_mapt replace_map, exprt &expr);
  bool build_custom_expr(const local_SSAt &SSA,
			 local_SSAt::nodest::const_iterator n_it,
			 exprt &expr);

  virtual void handle_special_functions(const local_SSAt &SSA);
  void instantiate_standard_domains(const local_SSAt &SSA);
  bool instantiate_custom_templates(const local_SSAt &SSA);

};


#endif
