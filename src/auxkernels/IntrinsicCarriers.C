#include "IntrinsicCarriers.h"

template <> InputParameters validParams<IntrinsicCarriers>()
{
  InputParameters params = validParams<AuxKernel>();
  params.addRequiredCoupledVar("temperature", "The material temperature");
  params.addRequiredParam<MaterialPropertyName>("conduction_band_DOS", "The density of states in the conduction band");
  params.addRequiredParam<MaterialPropertyName>("valence_band_DOS", "The density of states in the valence band");
  params.addRequiredParam<MaterialPropertyName>("band_gap_energy", "The energy of the band gap");
  params.addRequiredParam<Real>("k", "Boltzmann constant");
  params.addRequiredParam<Real>("q", "Elementary charge");
  return params;
}

IntrinsicCarriers::IntrinsicCarriers(const InputParameters & parameters)
  : DerivativeMaterialInterface<AuxKernel>(parameters),
  _temperature(coupledValue("temperature")),
  _Nc(getMaterialProperty<Real>("conduction_band_DOS")),
  _Nv(getMaterialProperty<Real>("valence_band_DOS")),
  _Eg(getMaterialProperty<Real>("band_gap_energy")),
  _k(getParam<Real>("k")),
  _q(getParam<Real>("q"))
{
}

Real
IntrinsicCarriers::computeValue()
{
  return 0.5 * (std::log(_Nc[_qp] * _Nv[_qp]) - _q * _Eg[_qp] / (_k * _temperature[_qp]));
}
