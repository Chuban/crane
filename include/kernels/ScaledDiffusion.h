#ifndef SCALEDDIFFUSION_H
#define SCALEDDIFFUSION_H

#include "Diffusion.h"
#include "SemiconductorUserObject.h"

// Forward Declaration
class ScaledDiffusion;

template <> InputParameters validParams<ScaledDiffusion>();

class ScaledDiffusion : public Diffusion
{
public:
  ScaledDiffusion(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

private:
  const SemiconductorUserObject & _semiconductor;
};

#endif // SCALEDDIFFUSION_H
