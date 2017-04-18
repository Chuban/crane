#ifndef CARRIERTRANSPORT_H
#define CARRIERTRANSPORT_H

#include "Kernel.h"
#include "SemiconductorUserObject.h"

// Forward Declaration
class CarrierTransport;

template <> InputParameters validParams<CarrierTransport>();

class CarrierTransport : public Kernel
{
public:
  CarrierTransport(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

  Real computeGamma();
  RealVectorValue computeGradGamma();
  RealVectorValue computeGradPhi();

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

#endif // CARRIERTRANSPORT_H
