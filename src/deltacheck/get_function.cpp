/*******************************************************************\

Module: Indexing

Author: Daniel Kroening, kroening@kroening.com

\*******************************************************************/

#include <goto-programs/read_goto_binary.h>

#include "get_function.h"

/*******************************************************************\

Function: get_functiont::operator()

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

const goto_functionst::goto_functiont * get_functiont::operator()(const irep_idt &id)
{
  // do we have it in our current file?
  if(current_file_name!="")
  {
    const goto_functionst::function_mapt::const_iterator
      f_it=goto_model.goto_functions.function_map.find(id);
    
    if(f_it!=goto_model.goto_functions.function_map.end())
      return &f_it->second; // found
  }
  
  // find in index
  indext::function_to_filet::const_iterator it=
    index.function_to_file.find(id);
   
  if(it==index.function_to_file.end())
    return NULL; // not there
  
  // pick first file
  assert(!it->second.empty());

  current_file_name=*(it->second.begin());
  
  status("Reading \""+id2string(current_file_name)+"\"");
  
  // read the file
  goto_model.clear();
  read_goto_binary(
    id2string(current_file_name),
    goto_model,
    get_message_handler());

  const goto_functionst::function_mapt::const_iterator
    f_it=goto_model.goto_functions.function_map.find(id);
  
  assert(f_it!=goto_model.goto_functions.function_map.end());
  return &f_it->second;
}
