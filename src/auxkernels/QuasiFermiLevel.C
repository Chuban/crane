#include "QuasiFermiLevel.h"

template <> InputParameters validParams<QuasiFermiLevel>()
{

  MooseEnum carrier_type("electron hole");
  MooseEnum statistics("Boltzmann Fermi-Dirac");
  InputParameters params = validParams<AuxKernel>();
  params.addRequiredCoupledVar("carrier", "The carrier concentration");
  params.addRequiredCoupledVar("potential", "The electric potential");
  params.addCoupledVar("temperature", "The material temperature");
  params.addRequiredParam<MooseEnum>("carrier_type", carrier_type, "The type of the carrier");
  params.addRequiredParam<MooseEnum>("statistics", statistics, "The type of carrier statistis to use");
  params.addRequiredParam<MaterialPropertyName>("intrinsic_concentration", "The intrinsic concentration material property name");
  params.addRequiredParam<Real>("k", "Boltzmann constant");
  params.addRequiredParam<Real>("q", "Elementary charge");
  return params;
}

QuasiFermiLevel::QuasiFermiLevel(const InputParameters & parameters)
  : DerivativeMaterialInterface<AuxKernel>(parameters),
  _statistics(getParam<MooseEnum>("statistics")),
  _carrier(getParam<MooseEnum>("carrier_type")),
  _carrier_concentration(coupledValue("carrier")),
  _potential(coupledValue("potential")),
  _temperature(isParamValid("temperature") ? coupledValue("temperature") : _zero),
  _intrinsic_concentration(getMaterialProperty<Real>("intrinsic_concentration")),
  _k(getParam<Real>("k")),
  _q(getParam<Real>("q"))
{
  if (_statistics == "Fermi-Dirac")
    mooseError("Fermi-Dirac statistics have not been implemented.");
}

Real
QuasiFermiLevel::computeValue()
{
  if (_statistics == "Boltzmann")
  {
    Real offset = _k * _temperature[_qp] * std::log(_carrier_concentration[_qp] / _intrinsic_concentration[_qp]) / _q;
    //mooseWarning(_potential[_qp] + offset);
    if (_carrier == "electron")
      return _potential[_qp] - offset;
    else
      return _potential[_qp] + offset;
  }
  else
    mooseError("Fermi-Dirac statistics have not been implemented.");

  return 0.;
}
