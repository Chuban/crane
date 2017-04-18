#ifndef INTRINSICCARRIERS_H
#define INTRINSICCARRIERS_H

// MOOSE includes
#include "AuxKernel.h"
#include "DerivativeMaterialInterface.h"

// Forward declarations
class IntrinsicCarriers;

template <>
InputParameters validParams<IntrinsicCarriers>();

class IntrinsicCarriers : public DerivativeMaterialInterface<AuxKernel>
{
public:
  IntrinsicCarriers(const InputParameters & parameters);

protected:
  virtual Real computeValue() override;

private:
  const VariableValue & _temperature;

  const MaterialProperty<Real> & _Nc;
  const MaterialProperty<Real> & _Nv;
  const MaterialProperty<Real> & _Eg;

  const Real _k;
  const Real _q;
};

#endif // INTRINSICCARRIERS_H
