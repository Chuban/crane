#ifndef SPACECHARGE_H
#define SPACECHARGE_H

#include "Kernel.h"
#include "SemiconductorUserObject.h"

// Forward Declaration
class SpaceCharge;

template <> InputParameters validParams<SpaceCharge>();

class SpaceCharge : public Kernel
{
public:
  SpaceCharge(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

private:
  unsigned int _n_tilde_var;
  const VariableValue & _n_tilde;
  unsigned int _p_tilde_var;
  const VariableValue & _p_tilde;
  unsigned int _T_var;
  const VariableValue & _T;

  const SemiconductorUserObject & _semiconductor;

  const Real _q;
};

#endif // SPACECHARGE_H
