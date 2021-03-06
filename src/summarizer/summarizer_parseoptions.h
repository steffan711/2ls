/*******************************************************************\

Module: Command Line Parsing

Author: Daniel Kroening, kroening@kroening.com

\*******************************************************************/

#ifndef CPROVER_SUMMARIZER_PARSEOPTIONS_H
#define CPROVER_SUMMARIZER_PARSEOPTIONS_H

#include <util/ui_message.h>
#include <util/parseoptions.h>

#include <langapi/language_ui.h>

class goto_modelt;
class optionst;

#include "summary_checker.h"

#define SUMMARIZER_OPTIONS \
  "(xml-ui)" \
  "(function):" \
  "D:I:" \
  "(depth):(context-bound):(unwind):" \
  "(bounds-check)(pointer-check)(div-by-zero-check)(memory-leak-check)" \
  "(signed-overflow-check)(unsigned-overflow-check)" \
  "(float-overflow-check)(nan-check)" \
  "(array-abstraction)" \
  "(non-incremental)" \
  "(no-assertions)(no-assumptions)" \
  "(16)(32)(64)(LP64)(ILP64)(LLP64)(ILP32)(LP32)" \
  "(little-endian)(big-endian)" \
  "(error-label):(verbosity):(no-library)" \
  "(version)" \
  "(i386-linux)(i386-macos)(i386-win32)(win32)(winx64)(gcc)" \
  "(ppc-macos)(unsigned-char)" \
  "(havoc)(intervals)(zones)(octagons)(equalities)"\
  "(enum-solver)(binsearch-solver)(arrays)"\
  "(string-abstraction)(no-arch)(arch):(floatbv)(fixedbv)" \
  "(round-to-nearest)(round-to-plus-inf)(round-to-minus-inf)(round-to-zero)" \
  "(inline)(inline-main)(inline-partial):" \
  "(context-sensitive)(termination)" \
  "(lexicographic-ranking-function):(monolithic-ranking-function)" \
  "(max-inner-ranking-iterations):" \
  "(preconditions)(sufficient)" \
  "(show-locs)(show-vcc)(show-properties)(show-trace)(show-fixed-points)(show-stats)" \
  "(show-goto-functions)(show-guards)(show-defs)(show-ssa)(show-assignments)" \
  "(show-invariants)" \
  "(property):(all-properties)(k-induction)(incremental-bmc)" \
  "(no-spurious-check)" \
  "(no-simplify)(no-fixed-point)" \
  "(no-unwinding-assertions)(no-propagation)" 
  // the last line is for CBMC-regression testing only

class summarizer_parseoptionst:
  public parseoptions_baset,
  public language_uit
{
public:
  virtual int doit();
  virtual void help();

  summarizer_parseoptionst(int argc, const char **argv);
  summarizer_parseoptionst(
    int argc,
    const char **argv,
    const std::string &extra_options);

protected:
  void get_command_line_options(optionst &options);

  bool get_goto_program(
    const optionst &options,
    goto_modelt &goto_model);

  bool process_goto_program(
    const optionst &options,
    goto_modelt &goto_model);
    
  bool set_properties(goto_modelt &);

  void report_success();
  void report_failure();
  void report_unknown();

  void report_properties(
    const goto_modelt &,
    const summary_checkert::property_mapt &);  

  void show_counterexample(
    const goto_modelt &,
    const class goto_tracet &);
          
  struct expr_statst {
    bool has_malloc;
    bool has_string;
    bool has_array;
    bool has_pointer;
    
    expr_statst()
    : has_malloc(false)
    , has_string(false)
    , has_array(false)
    , has_pointer(false)
    {}
  };    
   
  void type_stats_rec(
    const typet &type,
    expr_statst &stats,
    const namespacet &ns);
   
  void expr_stats_rec(
    const exprt &expr,
    expr_statst &stats);  
      
  void show_stats(
    const goto_modelt &,
    std::ostream &);    
            
  void eval_verbosity();

  void inline_main(goto_modelt &goto_model);
};

#endif
