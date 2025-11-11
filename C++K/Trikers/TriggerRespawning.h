#ifndef Triker_Respawning_H
#define Triker_Respawning_H

#include "Triker.h"


class TrikerRespawning : public Triker
{
protected:
  int   numUpdatesBetweenRespawns;
  int   numUpdatesRemainingUntilRespawn;

  //sets the triker to be inactive for numUpdatesBetweenRespawns 
  //update-steps
  void deactivate()
  {
    setInactive();
    numUpdatesRemainingUntilRespawn = numUpdatesBetweenRespawns;
  }

public:

  TrikerRespawning(TrikerRegion* region):Triker(region),
                             numUpdatesBetweenRespawns(0),
                             numUpdatesRemainingUntilRespawn(0)
  {}

  virtual ~TrikerRespawning(){}

  //to be implemented by child classes
  void  scan(Tasnal& role)  override
  {
  };
  //this is called each game-tick to update the triker's internal state
  virtual void update()
  {
    if ( (--numUpdatesRemainingUntilRespawn <= 0) && !isActive())
    {
      setActive();
    }
  }
  
  inline void setRespawnDelay(const unsigned int& numTicks)   {numUpdatesBetweenRespawns = numTicks;}
};




#endif