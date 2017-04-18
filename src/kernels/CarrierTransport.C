#include "CarrierTransport.h"

template <> InputParameters validParams<CarrierTransport>()
{
  MooseEnum carrier_type("electron hole");
  MooseEnum statistics("Boltzmann Fermi-Dirac");

  InputParameters params = validParams<Kernel>();

  params.addRequiredParam<MooseEnum>("carrier_type", carrier_type, "The type of the carrier");
  params.addRequiredParam<MooseEnum>("statistics", statistics, "The carrier statistics to use");

  params.addRequiredCoupledVar("potential", "The electrical potential");
  params.addRequiredCoupledVar("temperature", "The variable on which the coefficient may depend");

  params.addRequiredParam<UserObjectName>("semiconductor", "The semiconductor user object");

  params.addRequiredParam<Real>("k", "Boltzmann constant");
  params.addRequiredParam<Real>("q", "Elementary charge");
  return params;
}

CarrierTransport::CarrierTransport(const InputParameters & parameters)
  : Kernel(parameters),
  _carrier(getParam<MooseEnum>("carrier_type")),
  _statistics(getParam<MooseEnum>("statistics")),
  _psi_var(coupled("potential")),
  _grad_psi(coupledGradient("potential")),
  _T_var(coupled("temperature")),
  _T(coupledValue("temperature")),
  _grad_T(coupledGradient("temperature")),
  _semiconductor(getUserObject<SemiconductorUserObject>("semiconductor")),
  _k(getParam<Real>("k")),
  _q(getParam<Real>("q"))
{
  if (_statistics == "Fermi-Dirac")
    mooseError("Fermi-Dirac statistics have not been implemented.");
}

Real CarrierTransport::computeQpResidual()
{
  Real coeff;
  Real term1;
  Real term2;
  RealVectorValue gradPhi = computeGradPhi();

  if (_carrier == "electron")
    coeff = -1. * _semiconductor.getMun();
  else
    coeff = 1. * _semiconductor.getMup();

  term1 = coeff * _grad_test[_i][_qp] * gradPhi;
  term2 = -1. * coeff * _test[_i][_qp] * gradPhi * _grad_u[_qp];

  return term1 + term2;
}

Real CarrierTransport::computeQpJacobian()
{
  Real coeff;
  Real k_over_q;
  Real term1;
  Real term2;
  RealVectorValue gradPhi = computeGradPhi();
  RealVectorValue dgradPhidu;

  if (_carrier == "electron")
  {
    coeff = -1. * _semiconductor.getMun();
    k_over_q = -1. * _k / _q;
  }
  else
  {
    coeff = 1. * _semiconductor.getMup();
    k_over_q = _k / _q;
  }

  dgradPhidu = k_over_q * (_T[_qp] * _grad_phi[_j][_qp] + _grad_T[_qp] * _phi[_j][_qp]);

  term1 = coeff * _grad_test[_i][_qp] * dgradPhidu;
  term2 = -1 * coeff * _test[_i][_qp] * (gradPhi * _grad_phi[_j][_qp] + _grad_u[_qp] * dgradPhidu);

  return term1 + term2;
}

Real CarrierTransport::computeQpOffDiagJacobian(unsigned int jvar)
{
  Real coeff;
  Real k_over_q;
  Real term1;
  Real term2;
  RealVectorValue gradPhi = computeGradPhi();
  RealVectorValue dGradPhidT;
  RealVectorValue Nie = _semiconductor.getNieTilde(_T[_qp]);
  RealVectorValue gradNie = _semiconductor.getGradNieTilde(_T[_qp], _grad_T[_qp]);

  if (_carrier == "electron")
  {
    coeff = -1. * _semiconductor.getMun();
    k_over_q = -1. * _k / _q;
  }
  else
  {
    coeff = 1. * _semiconductor.getMup();
    k_over_q = _k / _q;
  }

  if (jvar == _psi_var)
  {
    term1 = coeff * _grad_test[_i][_qp] * _grad_phi[_j][_qp];
    term2 = -1 * coeff * _test[_i][_qp] * _grad_u[_qp] * _grad_phi[_j][_qp];
    return term1 + term2;
  }

  if (jvar == _T_var)
  {
    dGradPhidT = k_over_q * (_grad_u[_qp] +
                             _u[_qp] * _grad_phi[_j][_qp] -
                             gradNie -
                             Nie * _phi[_j][_qp]);

    term1 = coeff * _grad_test[_i][_qp] * dGradPhidT;
    term2 = -1 * coeff * _test[_i][_qp] * dGradPhidT * _grad_u[_qp];
    return term1 + term2;
  }

  return 0.;
}

RealVectorValue CarrierTransport::computeGradPhi()
{
  Real coeff;
  Real gamma = _semiconductor.getGamma();
  RealVectorValue gradGamma = _semiconductor.getGradGamma();
  Real nie = _semiconductor.getNieTilde(_T[_qp]);
  RealVectorValue gradNie = _semiconductor.getGradNieTilde(_T[_qp], _grad_T[_qp]);
  RealVectorValue gradPhi;

  if (_carrier == "electron")
    coeff = -1. * _k / _q;
  else
    coeff = _k / _q;

  gradPhi = _grad_psi[_qp] + coeff * (_T[_qp] * _grad_u[_qp] +
                                      _u[_qp] * _grad_T[_qp] -
                                      _T[_qp] * gradNie -
                                      nie * _grad_T[_qp] -
                                      _T[_qp] * gradGamma / gamma -
                                      std::log(gamma) * _grad_T[_qp]);

  return gradPhi;
}
