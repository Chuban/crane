#include "SemiconductorUserObject.h"

template <> InputParameters validParams<SemiconductorUserObject>()
{
  MooseEnum statistics("Boltzmann Fermi-Dirac");
  InputParameters params = validParams<GeneralUserObject>();

  params.addRequiredParam<MooseEnum>("statistics", statistics, "The carrier statistics to be used");

  params.addParam<Real>("Nd", 0, "Donor concentration");
  params.addParam<Real>("Nd_energy", 0, "Donor energy level");
  params.addParam<Real>("Nd_degen", 0, "Donor degeneracy");

  params.addParam<Real>("Na", 0, "Acceptor concentration");
  params.addParam<Real>("Na_energy", 0, "Acceptor energy level");
  params.addParam<Real>("Na_degen", 0, "Acceptor degeneracy");

  params.addRequiredParam<Real>("Eg", "The band gap energy");
  params.addRequiredParam<Real>("alpha", "A band gap scaling parameter (1/K)");
  params.addRequiredParam<Real>("beta", "A band gap scaling parameter (K)");

  params.addRequiredParam<Real>("Nc", "The density of states in the conduction band at T=1K");
  params.addRequiredParam<Real>("Nv", "The density of states in the valence band at T=1K");

  params.addRequiredParam<Real>("electron_mobility", "Electron mobility");
  params.addRequiredParam<Real>("hole_mobility", "Hole mobility");

  params.addParam<Real>("A_n**", 1., "The effective electron Richardson constant");
  params.addParam<Real>("A_p**", 1., "The effective hole Richardson constant");

  params.addParam<Real>("epsilon_r", 1., "The relative permittivity of the semiconductor");

  params.addRequiredParam<Real>("k", "Boltzmann constant");
  params.addRequiredParam<Real>("q", "Elementary charge");
  params.addRequiredParam<Real>("epsilon_0", "The permittivity of free space");

  return params;
}

SemiconductorUserObject::SemiconductorUserObject(const InputParameters & parameters)
  : GeneralUserObject(parameters),
  _statistics(getParam<MooseEnum>("statistics")),
  _Nd(getParam<Real>("Nd")),
  _Nd_energy(getParam<Real>("Nd_energy")),
  _Nd_degen(getParam<Real>("Nd_degen")),
  _Na(getParam<Real>("Na")),
  _Na_energy(getParam<Real>("Na_energy")),
  _Na_degen(getParam<Real>("Na_degen")),
  _Eg0(getParam<Real>("Eg")),
  _alpha(getParam<Real>("alpha")),
  _beta(getParam<Real>("beta")),
  _Nc0(getParam<Real>("Nc")),
  _Nv0(getParam<Real>("Nv")),
  _mu_n(getParam<Real>("electron_mobility")),
  _mu_p(getParam<Real>("hole_mobility")),
  _A_n(getParam<Real>("A_n**")),
  _A_p(getParam<Real>("A_p**")),
  _epsilon_r(getParam<Real>("epsilon_r")),
  _k(getParam<Real>("k")),
  _q(getParam<Real>("q")),
  _epsilon_0(getParam<Real>("epsilon_0"))
{
  if (_statistics == "Fermi-Dirac")
    mooseError("Fermi-Dirac statistics have not been implemented.");
}

void SemiconductorUserObject::initialize()
{
}

void SemiconductorUserObject::execute()
{
}

void SemiconductorUserObject::finalize()
{
}

void SemiconductorUserObject::threadJoin(const UserObject & /*y*/)
{
}

Real SemiconductorUserObject::getNd(Real phi_n, Real T) const
{
  if (_Nd_degen == 0)
    return _Nd;
  else
    return _Nd / (1 + _Nd_degen * std::exp(-1. * (_q * phi_n + _Nd_energy) / (_k * T)));
}

Real SemiconductorUserObject::getNa(Real phi_p, Real T) const
{
  if (_Na_degen == 0)
    return _Na;
  else
    return _Na / (1 + _Na_degen * std::exp((_q * phi_p + _Na_energy) / (_k * T)));
}

Real SemiconductorUserObject::getEg(Real T) const
{
  return _Eg0 + _alpha * std::pow(T, 2) / (T + _beta);
}

Real SemiconductorUserObject::getdEgdT(Real T) const
{
  return _alpha * T * (T + 2 * _beta) / std::pow(T + _beta, 2);
}

Real SemiconductorUserObject::getNc(Real T) const
{
  return _Nc0 * std::pow(T, 1.5);
}

Real SemiconductorUserObject::getdNcdT(Real T) const
{
  return 1.5 * _Nc0 * std::sqrt(T);
}

Real SemiconductorUserObject::getNv(Real T) const
{
  return _Nv0 * std::pow(T, 1.5);
}

Real SemiconductorUserObject::getdNvdT(Real T) const
{
  return 1.5 * _Nv0 * std::sqrt(T);
}

Real SemiconductorUserObject::getGamma() const
{
  if (_statistics == "Fermi-Dirac")
    mooseError("Fermi-Dirac statistics have no been implemented");

  return 1.;
}

RealVectorValue SemiconductorUserObject::getGradGamma() const
{
  if (_statistics == "Fermi-Dirac")
    mooseError("Fermi-Dirac statistics have no been implemented");

  RealVectorValue zeroGrad(0., 0., 0.);
  return zeroGrad;
}

Real SemiconductorUserObject::getNie(Real T) const
{
  return std::sqrt(getNc(T) * getNv(T)) * std::exp(-1. * _q * getEg(T) / (2 * _k * T));
}

Real SemiconductorUserObject::getNieTilde(Real T) const
{
  return 0.5 * std::log(getNc(T) * getNv(T)) - _q * getEg(T) / (2 * _k * T);
}

RealVectorValue SemiconductorUserObject::getGradNieTilde(Real T, RealVectorValue gradT) const
{
  RealVectorValue gradNieTilde = 0.5 * (getdNcdT(T) * gradT / getNc(T) + getdNvdT(T) * gradT / getNv(T));
  gradNieTilde -= (_q / _k) * (T * getdEgdT(T) - getEg(T)) * gradT / std::pow(T, 2);
  return gradNieTilde;
}

Real SemiconductorUserObject::getMun() const
{
  return _mu_n;
}

Real SemiconductorUserObject::getMup() const
{
  return _mu_p;
}

Real SemiconductorUserObject::getAn() const
{
  return _A_n;
}

Real SemiconductorUserObject::getAp() const
{
  return _A_p;
}

Real SemiconductorUserObject::getEpsilon() const
{
  return _epsilon_r * _epsilon_0;
}
