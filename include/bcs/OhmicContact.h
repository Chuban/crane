#ifndef OHMICCONTACT_H
#define OHMICCONTACT_H

#include "NodalBC.h"
#include "SemiconductorUserObject.h"

// Forward Declarations
class OhmicContact;

template <> InputParameters validParams<OhmicContact>();

class OhmicContact : public NodalBC {
public:
  OhmicContact(const InputParameters &parameters);

protected:
  virtual Real computeQpResidual();

private:
  MooseEnum _statistics;
  MooseEnum _contact_element;

  const VariableValue & _temperature;
  const Real & _potential;

  const SemiconductorUserObject & _semiconductor;

  const Real _k;
  const Real _q;
};

#endif // OHMICCONTACT_H
