#ifndef TRIGGER_LIMITEDLIFETIME_H
#define TRIGGER_LIMITEDLIFETIME_H

#include "Triker.h"
template <class entity_type>
class TrikerLimitedLifetime : public Triker<entity_type>
{
protected:

  //the lifetime of this triker in update-steps
  int lifeTime;

public:

  TrikerLimitedLifetime(int lifetime):Triker<entity_type>(1),
                                        lifeTime(lifetime)
  {}

  virtual ~TrikerLimitedLifetime(){}
  //children of this class should always make sure this is called from within
  //their own update method
  virtual void update()
  {
    //if the lifetime counter expires set this triker to be removed from
    if (--lifeTime <= 0)
    {
      remove();
    }
  }

  //to be implemented by child classes
  virtual void  scan(entity_type*) = 0 ;
};




#endif