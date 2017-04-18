#ifndef SEMICONDUCTORMATERIAL_H
#define SEMICONDUCTORMATERIAL_H


#include "DerivativeMaterialInterface.h"
#include "Material.h"
#include "SemiconductorUserObject.h"

// Forward Declarations
class SemiconductorMaterial;

template <> InputParameters validParams<SemiconductorMaterial>();

class SemiconductorMaterial : public DerivativeMaterialInterface<Material> {
public:
  SemiconductorMaterial(const InputParameters &parameters);

protected:
  virtual void computeQpProperties();

private:
  const SemiconductorUserObject & _semiconductor;
  const VariableValue & _T;
  const VariableGradient & _grad_T;

  MaterialProperty<Real> & _mu_n;
  MaterialProperty<Real> & _mu_p;

  MaterialProperty<Real> & _donor_ionized_impurity;
  MaterialProperty<Real> & _d_donor_ionized_impurity_dT;

  MaterialProperty<Real> & _acceptor_ionized_impurity;
  MaterialProperty<Real> & _d_acceptor_ionized_impurity_dT;

  MaterialProperty<Real> & _band_gap_energy;
  MaterialProperty<Real> & _d_band_gap_energy_dT;
  MaterialProperty<Real> & _conduction_DOS;
  MaterialProperty<Real> & _d_conduction_DOS_dT;
  MaterialProperty<Real> & _valence_DOS;
  MaterialProperty<Real> & _d_valence_DOS_dT;

  MaterialProperty<Real> & _A_n_star_star;
  MaterialProperty<Real> & _A_p_star_star;
};

#endif // SEMICONDUCTORMATERIAL_H
