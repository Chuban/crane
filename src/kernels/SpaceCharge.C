#include "SpaceCharge.h"

template <> InputParameters validParams<SpaceCharge>()
{
  InputParameters params = validParams<Kernel>();
  params.addRequiredCoupledVar("n_tilde", "The logarithm of the electron concentration");
  params.addRequiredCoupledVar("p_tilde", "The logarithm of the hole concentration");
  params.addRequiredCoupledVar("temperature", "The material temperature");
  params.addRequiredParam<UserObjectName>("semiconductor", "The semiconductor user object");
  params.addRequiredParam<Real>("q", "Elementary charge");
  return params;
}

SpaceCharge::SpaceCharge(const InputParameters & parameters)
  : Kernel(parameters),
  _n_tilde_var(coupled("n_tilde")),
  _n_tilde(coupledValue("n_tilde")),
  _p_tilde_var(coupled("p_tilde")),
  _p_tilde(coupledValue("p_tilde")),
  _T_var(coupled("temperature")),
  _T(coupledValue("temperature")),
  _semiconductor(getUserObject<SemiconductorUserObject>("semiconductor")),
  _q(getParam<Real>("q"))
{
}

Real SpaceCharge::computeQpResidual()
{
  Real Nd = _semiconductor.getNd(0., _T[_qp]);
  Real Na = _semiconductor.getNa(0., _T[_qp]);
  return _q * (std::exp(_p_tilde[_qp]) - std::exp(_n_tilde[_qp]) + Nd - Na);
}

Real SpaceCharge::computeQpJacobian()
{
  // TODO: Both Nd and Na may depend of psi.
  return 0;
}

Real SpaceCharge::computeQpOffDiagJacobian(unsigned int jvar)
{
  if (jvar == _n_tilde_var)
    return _q * std::exp(_n_tilde[_qp]);

  if (jvar == _p_tilde_var)
    return -1. * _q * std::exp(_p_tilde[_qp]);

  // TODO: Both Nd and Na may depend on T.
  if (jvar == _T_var)
    return -1. * _q * (0. - 0.);

  return 0;
}
