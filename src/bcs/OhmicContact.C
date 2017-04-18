#include "OhmicContact.h"

template <> InputParameters validParams<OhmicContact>()
{
  InputParameters params = validParams<NodalBC>();

  MooseEnum statistics("Boltzmann Fermi-Dirac");
  params.addRequiredParam<MooseEnum>("statistics", statistics, "Choose the semiconductor statistics");

  MooseEnum contact_element("potential electron hole");
  params.addRequiredParam<MooseEnum>("contact_element", contact_element, "Choose the contact element");

  params.addRequiredCoupledVar("temperature", "The temperature of the material");
  params.addRequiredParam<Real>("potential", "The electrical potential of the BC");
  params.addRequiredParam<UserObjectName>("semiconductor", "The semiconductor user object");

  params.addRequiredParam<Real>("k", "Boltzmann constant");
  params.addRequiredParam<Real>("q", "Elementary charge");

  return params;
}

OhmicContact::OhmicContact(const InputParameters & parameters)
  : NodalBC(parameters),
  _statistics(getParam<MooseEnum>("statistics")),
  _contact_element(getParam<MooseEnum>("contact_element")),
  _temperature(coupledValue("temperature")),
  _potential(getParam<Real>("potential")),
  _semiconductor(getUserObject<SemiconductorUserObject>("semiconductor")),
  _k(getParam<Real>("k")),
  _q(getParam<Real>("q"))
{
  if (_statistics == "Fermi-Dirac")
    mooseError("Fermi-Dirac statistics have not been implemented.");
}

Real OhmicContact::computeQpResidual()
{
  Real n, p, V;

  // The intrinsic carrier concentration comes to use as n_ie_tilde, but we need n_ie.
  Real Nd = _semiconductor.getNd(0, _temperature[_qp]);
  Real Na = _semiconductor.getNa(0, _temperature[_qp]);
  Real n_ie = _semiconductor.getNie(_temperature[_qp]);

  n = 0.5 * (Nd - Na + std::sqrt( std::pow(Nd - Na, 2) + 4 * std::pow(n_ie, 2)));
  p = std::pow(n_ie, 2) / n;
  V = _potential + _k * _temperature[_qp] * std::log(n / n_ie) / _q;

  // We have computed n and p, but we need n_tilde and p_tilde
  n = std::log(n);
  p = std::log(p);

  switch (_contact_element)
  {
    case 0 : return _u[_qp] - V; // potential
    case 1 : return _u[_qp] - n; // electrons
    case 2 : return _u[_qp] - p; // holes
    default : return 0.;
  }
}
