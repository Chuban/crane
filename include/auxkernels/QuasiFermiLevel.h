#ifndef QUASIFERMILEVEL_H
#define QUASIFERMILEVEL_H

// MOOSE includes
#include "AuxKernel.h"
#include "DerivativeMaterialInterface.h"

// Forward declarations
class QuasiFermiLevel;

template <>
InputParameters validParams<QuasiFermiLevel>();

class QuasiFermiLevel : public DerivativeMaterialInterface<AuxKernel>
{
public:
  QuasiFermiLevel(const InputParameters & parameters);

protected:
  virtual Real computeValue() override;

private:
  MooseEnum _statistics;
  MooseEnum _carrier;

  const VariableValue & _carrier_concentration;
  const VariableValue & _potential;
  const VariableValue & _temperature;

  const MaterialProperty<Real> & _intrinsic_concentration;

  const Real _k;
  const Real _q;
};

#endif // QUASIFERMILEVEL_H
