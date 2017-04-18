#ifndef SEMICONDUCTORUSEROBJECT_H
#define SEMICONDUCTORUSEROBJECT_H

#include "GeneralUserObject.h"

class SemiconductorUserObject;

template <> InputParameters validParams<SemiconductorUserObject>();

class SemiconductorUserObject : public GeneralUserObject
{
public:
  SemiconductorUserObject(const InputParameters & parameters);

  virtual void initialize();
  virtual void execute();
  virtual void finalize();
  virtual void threadJoin(const UserObject & y);

  Real getNd(Real, Real) const;

  Real getNa(Real, Real) const;

  Real getEg(Real) const;
  Real getdEgdT(Real T) const;

  Real getNc(Real) const;
  Real getdNcdT(Real) const;

  Real getNv(Real) const;
  Real getdNvdT(Real) const;

  Real getGamma() const;
  RealVectorValue getGradGamma() const;

  Real getNie(Real) const;
  Real getNieTilde(Real) const;
  RealVectorValue getGradNieTilde(Real, RealVectorValue) const;

  Real getMun() const;
  Real getMup() const;

  Real getAn() const;
  Real getAp() const;

  Real getEpsilon() const;

protected:
  MooseEnum _statistics;

  Real _Nd;
  Real _Nd_energy;
  Real _Nd_degen;

  Real _Na;
  Real _Na_energy;
  Real _Na_degen;

  Real _Eg0;
  Real _alpha;
  Real _beta;

  Real _Nc0;
  Real _Nv0;

  Real _mu_n;
  Real _mu_p;

  Real _A_n;
  Real _A_p;

  Real _epsilon_r;

  Real _k;
  Real _q;
  Real _epsilon_0;
};

#endif // SEMICONDUCTORUSEROBJECT_H
