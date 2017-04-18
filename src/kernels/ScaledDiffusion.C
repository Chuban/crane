#include "ScaledDiffusion.h"

template <> InputParameters validParams<ScaledDiffusion>()
{
  InputParameters params = validParams<Diffusion>();
  params.addRequiredParam<UserObjectName>("semiconductor", "The semiconductor user object");
  return params;
}

ScaledDiffusion::ScaledDiffusion(const InputParameters & parameters)
  : Diffusion(parameters),
  _semiconductor(getUserObject<SemiconductorUserObject>("semiconductor"))
{
}

Real ScaledDiffusion::computeQpResidual()
{
  return _semiconductor.getEpsilon() * Diffusion::computeQpResidual();
}

Real ScaledDiffusion::computeQpJacobian()
{
  return _semiconductor.getEpsilon() * Diffusion::computeQpJacobian();
}

Real ScaledDiffusion::computeQpOffDiagJacobian(unsigned int /*jvar*/)
{
  return 0.;
}
