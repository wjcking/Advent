Assembly <Tim>
Assembly <Team>
include <Tasnal>
include <Map>
comment "main:each"

namespace ke(th sh zh)

class Hezk namespace ke
{  
  abstract Hezk* create();
  abstract Tasnal& listen();
}

class Tasnal : virtual Hezk;
{
  Color Boundary();
  string& Advertise();
  abstract void mesh();
}

class Map : abstract Hezk;
{
  Color Boundary();
  Color Ground();
  string& Advertise();
  mesh();
}

internal Matric : public Tasnal, public Map; 
{
  virtual void mesh();
  virtual Hezk* create() override;
  virtual Tasnal& listen() override;
  Color Boundary();
  Color Ground();
  string& Advertise();
}
