/*******************************************************************\

Module: Discover the Guards of Basic Blocks

Author: Daniel Kroening, kroening@kroening.com

\*******************************************************************/

#ifndef CPROVER_GUARD_DOMAIN_H
#define CPROVER_GUARD_DOMAIN_H

#include <analyses/ai.h>

class guard_domaint:public ai_domain_baset
{
public:
  // the constructor builds 'bottom'
  inline guard_domaint():unreachable(true)
  {
  }

  // A guard may be one of the following:
  // 1) a location of a branch, possibly negated for the else-case
  // 2) a location of a merged guard
  struct guardt
  {
  public:
    locationt loc;
    
    enum { NONE, BRANCH_TAKEN, BRANCH_NOT_TAKEN, MERGED } kind;
    
    guardt():kind(NONE)
    {
    }
    
    explicit guardt(locationt _loc):loc(_loc), kind(MERGED)
    {
    }

    guardt(locationt branch, bool truth):
      loc(branch), kind(truth?BRANCH_TAKEN:BRANCH_NOT_TAKEN)
    {
    }
    
    inline bool is_branch() const
    {
      return kind==BRANCH_TAKEN || kind==BRANCH_NOT_TAKEN;
    }
  };
  
  inline friend bool operator==(const guardt &a, const guardt &b)
  {
    return a.kind==b.kind &&
           a.loc->location_number==b.loc->location_number;
  }
  
  inline friend bool operator!=(const guardt &a, const guardt &b)
  {
    return !(a==b);
  }
  
  virtual void make_entry_state()
  {
    // this is 'top'
    unreachable=false;
    guards.clear();
  }

  // We may be under some set of guards.
  typedef std::vector<guardt> guardst;
  guardst guards;
  
  bool unreachable;
  
  // Keep the guards for all incoming edges.
  typedef std::map<locationt, guardst> incomingt;
  incomingt incoming;
  
  // returns true iff 'a' and 'b' match in all but the last place
  static bool prefix_match(const guardst &a, const guardst &b)
  {
    if(a.size()!=b.size() || a.empty()) return false;
    for(unsigned i=0; i<a.size()-1; i++)
      if(a[i]!=b[i]) return false;
    return true;
  }

  virtual void transform(
    const namespacet &ns,
    locationt from,
    locationt to);
              
  virtual void output(
    const namespacet &ns,
    std::ostream &out) const;

  bool merge(
    const guard_domaint &b,
    locationt from,
    locationt to);
};

#endif
