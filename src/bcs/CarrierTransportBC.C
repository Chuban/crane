#include "CarrierTransportBC.h"

template <> InputParameters validParams<CarrierTransportBC>()
{
  MooseEnum carrier_type("electron hole");
  MooseEnum statistics("Boltzmann Fermi-Dirac");

  InputParameters params = validParams<IntegratedBC>();

  params.addRequiredParam<MooseEnum>("carrier_type", carrier_type, "The type of the carrier");
  params.addRequiredParam<MooseEnum>("statistics", statistics, "The carrier statistics to use");

  params.addRequiredCoupledVar("potential", "The electrical potential");
  params.addRequiredCoupledVar("temperature", "The variable on which the coefficient may depend");

  params.addRequiredParam<UserObjectName>("semiconductor", "The semiconductor user object");

  params.addRequiredParam<Real>("k", "Boltzmann constant");
  params.addRequiredParam<Real>("q", "Elementary charge");
  return params;
}

CarrierTransportBC::CarrierTransportBC(const InputParameters & parameters)
  : IntegratedBC(parameters),
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

Real CarrierTransportBC::computeQpResidual()
{
  Real coeff;
  Real gamma = _semiconductor.getGamma();
  RealVectorValue gradGamma = _semiconductor.getGradGamma();
  Real nie = _semiconductor.getNieTilde(_T[_qp]);
  RealVectorValue gradNie = _semiconductor.getGradNieTilde(_T[_qp], _grad_T[_qp]);
  RealVectorValue value;

  if (_carrier == "electron")
    coeff = _k / _q;
  else
    coeff = -1. * _k / _q;

  value = (coeff * _grad_psi[_qp] -
           _u[_qp] * _grad_T[_qp] +
           _T[_qp] * gradNie +
           nie * _grad_T[_qp] +
           _T[_qp] * gradGamma +
           std::log(gamma) * _grad_T[_q]) / _T[_qp];

  return -_test[_i][_qp] * value * _normals[_qp];
}
