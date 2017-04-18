#include "SemiconductorMaterial.h"

template <> InputParameters validParams<SemiconductorMaterial>() {

  InputParameters params = validParams<Material>();

  params.addRequiredParam<UserObjectName>("semiconductor", "The semiconductor user object");
  params.addRequiredCoupledVar("temperature", "The variable on which the coefficient may depend");

  return params;
}

SemiconductorMaterial::SemiconductorMaterial(const InputParameters &parameters)
    : DerivativeMaterialInterface<Material>(parameters),

    _semiconductor(getUserObject<SemiconductorUserObject>("semiconductor")),

    _T(coupledValue("temperature")),
    _grad_T(coupledGradient("temperature")),

    _mu_n(declareProperty<Real>("mu_n")),
    _mu_p(declareProperty<Real>("mu_p")),

    _donor_ionized_impurity(declareProperty<Real>("donor_ionized_impurity")),
    _d_donor_ionized_impurity_dT(declarePropertyDerivative<Real>("donor_ionized_impurity", getVar("temperature", 0)->name())),
    _acceptor_ionized_impurity(declareProperty<Real>("acceptor_ionized_impurity")),
    _d_acceptor_ionized_impurity_dT(declarePropertyDerivative<Real>("acceptor_ionized_impurity", getVar("temperature", 0)->name())),

    _band_gap_energy(declareProperty<Real>("band_gap_energy")),
    _d_band_gap_energy_dT(declarePropertyDerivative<Real>("band_gap_energy", getVar("temperature", 0)->name())),
    _conduction_DOS(declareProperty<Real>("conduction_DOS")),
    _d_conduction_DOS_dT(declarePropertyDerivative<Real>("conduction_DOS", getVar("temperature", 0)->name())),
    _valence_DOS(declareProperty<Real>("valence_DOS")),
    _d_valence_DOS_dT(declarePropertyDerivative<Real>("valence_DOS", getVar("temperature", 0)->name())),

    _A_n_star_star(declareProperty<Real>("A_n")),
    _A_p_star_star(declareProperty<Real>("A_p"))
{
}

void SemiconductorMaterial::computeQpProperties()
{
  _mu_n[_qp] = _semiconductor.getMun();
  _mu_p[_qp] = _semiconductor.getMup();

  // TODO: Fix these.
  _donor_ionized_impurity[_qp] = _semiconductor.getNd(0., _T[_qp]);
  _d_donor_ionized_impurity_dT[_qp] = 0.;
  _acceptor_ionized_impurity[_qp] = _semiconductor.getNa(0., _T[_qp]);
  _d_acceptor_ionized_impurity_dT[_qp] = 0.;

  _band_gap_energy[_qp] = _semiconductor.getEg(_T[_qp]);
  _d_band_gap_energy_dT[_qp] = _semiconductor.getdEgdT(_T[_qp]);
  _conduction_DOS[_qp] = _semiconductor.getNc(_T[_qp]);
  _d_conduction_DOS_dT[_qp] = _semiconductor.getdNcdT(_T[_qp]);
  _valence_DOS[_qp] = _semiconductor.getNv(_T[_qp]);
  _d_valence_DOS_dT[_qp] = _semiconductor.getdNvdT(_T[_qp]);

  _A_n_star_star[_qp] = _semiconductor.getAn();
  _A_p_star_star[_qp] = _semiconductor.getAp();
}
