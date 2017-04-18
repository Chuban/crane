#ifndef CARRIERTRANSPORTBC_H
#define CARRIERTRANSPORTBC_H

#include "IntegratedBC.h"
#include "SemiconductorUserObject.h"

class CarrierTransportBC;

template <>
InputParameters validParams<CarrierTransportBC>();

class CarrierTransportBC : public IntegratedBC
{
public:
  CarrierTransportBC(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual() override;

private:
  MooseEnum _carrier;
  MooseEnum _statistics;

  unsigned int _psi_var;
  const VariableGradient & _grad_psi;
  unsigned int _T_var;
  const VariableValue & _T;
  const VariableGradient & _grad_T;

  const SemiconductorUserObject & _semiconductor;

  const Real _k;
  const Real _q;
};

#endif // CARRIERTRANSPORTBC_H
