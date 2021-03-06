/*******************************************************************\

Module: Template Generator for Ranking Functions

Author: Peter Schrammel

\*******************************************************************/

#include "template_generator_ranking.h"

#include "linrank_domain.h"
#include "lexlinrank_domain.h"

#include <util/find_symbols.h>
#include <util/arith_tools.h>
#include <util/simplify_expr.h>
#include <util/mp_arith.h>

#ifdef DEBUG
#include <iostream>
#endif

/*******************************************************************\

Function: template_generator_rankingt::operator()

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

void template_generator_rankingt::operator()(unsigned _domain_number,
			  const local_SSAt &SSA,  bool forward)
{
  domain_number = _domain_number;
  handle_special_functions(SSA); // we have to call that to prevent trouble!

  if(options.get_bool_option("monolithic-ranking-function"))
  {
    domain_ptr = new linrank_domaint(domain_number,post_renaming_map, SSA.ns);
  }
  else
  {
    domain_ptr = new lexlinrank_domaint(domain_number,post_renaming_map, SSA.ns);
  }
 collect_variables_ranking(SSA,forward);

 options.set_option("compute-ranking-functions",true);

#if 1
  debug() << "Template variables: " << eom;
  domaint::output_var_specs(debug(),var_specs,SSA.ns); debug() << eom;
  debug() << "Template: " << eom;
  domain_ptr->output_domain(debug(), SSA.ns); debug() << eom;
#endif  
}

/*******************************************************************\

Function: template_generator_rankingt::collect_variables_ranking

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

void template_generator_rankingt::collect_variables_ranking(const local_SSAt &SSA,bool forward)
{
  // used for renaming map
  var_listt pre_state_vars, post_state_vars;

  // add loop variables
  for(local_SSAt::nodest::const_iterator n_it = SSA.nodes.begin(); 
      n_it != SSA.nodes.end(); n_it++)
  {
    if(n_it->loophead != SSA.nodes.end()) //we've found a loop
    {
      domaint::var_specst new_var_specs;

      exprt lhguard = SSA.guard_symbol(n_it->loophead->location);
      ssa_local_unwinder.unwinder_rename(to_symbol_expr(lhguard),*n_it,true);
      exprt lsguard = SSA.name(SSA.guard_symbol(), local_SSAt::LOOP_SELECT, n_it->location);
      ssa_local_unwinder.unwinder_rename(to_symbol_expr(lsguard),*n_it,true);
      exprt pre_guard = lhguard; //and_exprt(lhguard,lsguard);

      exprt pguard = SSA.guard_symbol(n_it->location);
      ssa_local_unwinder.unwinder_rename(to_symbol_expr(pguard),*n_it,false);
      exprt pcond = SSA.cond_symbol(n_it->location);
      ssa_local_unwinder.unwinder_rename(to_symbol_expr(pcond),*n_it,false);
      exprt post_guard = and_exprt(pguard,pcond);
      
      const ssa_domaint::phi_nodest &phi_nodes = 
        SSA.ssa_analysis[n_it->loophead->location].phi_nodes;
      
      // Record the objects modified by the loop to get
      // 'primed' (post-state) and 'unprimed' (pre-state) variables.
      for(local_SSAt::objectst::const_iterator
          o_it=SSA.ssa_objects.objects.begin();
          o_it!=SSA.ssa_objects.objects.end();
          o_it++)
      {
        ssa_domaint::phi_nodest::const_iterator p_it=
        phi_nodes.find(o_it->get_identifier());

	if(p_it==phi_nodes.end()) continue; // object not modified in this loop

//        symbol_exprt in=SSA.name(*o_it, local_SSAt::LOOP_BACK, n_it->location);
        symbol_exprt in=SSA.name(*o_it, local_SSAt::PHI, n_it->loophead->location);
        ssa_local_unwinder.unwinder_rename(in,*n_it,true);
        symbol_exprt out=SSA.read_rhs(*o_it, n_it->location);
        ssa_local_unwinder.unwinder_rename(out,*n_it,false);

        add_var(in,pre_guard,post_guard,domaint::LOOP,new_var_specs);
      
        // building map for renaming from pre into post-state
        post_renaming_map[in] = out;    
       
  #ifdef DEBUG
        std::cout << "Adding " << from_expr(ns, "", in) << " " << 
          from_expr(ns, "", out) << std::endl;        
  #endif
      }

      filter_ranking_domain(new_var_specs);

#ifndef LEXICOGRAPHIC
      static_cast<linrank_domaint *>(domain_ptr)->add_template(
        new_var_specs, SSA.ns);
#else
      static_cast<lexlinrank_domaint *>(domain_ptr)->add_template(
        new_var_specs, SSA.ns);
#endif

      var_specs.insert(var_specs.end(),new_var_specs.begin(),new_var_specs.end());
    } 
  }
}

/*******************************************************************\

Function: template_generator_rankingt::filter_ranking_domain

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

void template_generator_rankingt::filter_ranking_domain(domaint::var_specst &var_specs)
{
  domaint::var_specst new_var_specs(var_specs);
  var_specs.clear();
  for(domaint::var_specst::const_iterator v = new_var_specs.begin(); 
      v!=new_var_specs.end(); v++)
  {
    const domaint::vart &s = v->var;
    if(s.type().id()==ID_unsignedbv || s.type().id()==ID_signedbv ||
       s.type().id()==ID_floatbv)
    {
      var_specs.push_back(*v);
    }
#if 0
    if(s.type().id()==ID_pointer)
    {
      domaint::var_spect new_varspec = *v;
      new_varspec.var = typecast_exprt(v->var,to_pointer_type(v->var.type()).subtype());
      var_specs.push_back(new_varspec);
    }
#endif
  }
}
