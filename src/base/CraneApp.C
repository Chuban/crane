#include "CraneApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "ModulesApp.h"
#include "MooseSyntax.h"

#include "SemiconductorMaterial.h"
#include "OhmicContact.h"
#include "ScaledDiffusion.h"
#include "QuasiFermiLevel.h"
#include "CarrierTransport.h"
#include "SpaceCharge.h"
#include "IntrinsicCarriers.h"
#include "SemiconductorUserObject.h"
#include "CarrierTransportBC.h"

template<>
InputParameters validParams<CraneApp>()
{
  InputParameters params = validParams<MooseApp>();
  return params;
}

CraneApp::CraneApp(InputParameters parameters) :
    MooseApp(parameters)
{
  Moose::registerObjects(_factory);
  ModulesApp::registerObjects(_factory);
  CraneApp::registerObjects(_factory);

  Moose::associateSyntax(_syntax, _action_factory);
  ModulesApp::associateSyntax(_syntax, _action_factory);
  CraneApp::associateSyntax(_syntax, _action_factory);
}

CraneApp::~CraneApp()
{
}

// External entry point for dynamic application loading
extern "C" void CraneApp__registerApps() { CraneApp::registerApps(); }
void
CraneApp::registerApps()
{
  registerApp(CraneApp);
}

// External entry point for dynamic object registration
extern "C" void CraneApp__registerObjects(Factory & factory) { CraneApp::registerObjects(factory); }
void
CraneApp::registerObjects(Factory & factory)
{
  // Materials
  registerMaterial(SemiconductorMaterial);

  // Boundary conditions
  registerBoundaryCondition(OhmicContact);
  registerBoundaryCondition(CarrierTransportBC);

  // Kernels
  registerKernel(ScaledDiffusion);
  registerKernel(CarrierTransport);
  registerKernel(SpaceCharge);

  // Aux Kernels
  registerAuxKernel(QuasiFermiLevel);
  registerAuxKernel(IntrinsicCarriers);

  // User objects
  registerUserObject(SemiconductorUserObject);
}

// External entry point for dynamic syntax association
extern "C" void CraneApp__associateSyntax(Syntax & syntax, ActionFactory & action_factory) { CraneApp::associateSyntax(syntax, action_factory); }
void
CraneApp::associateSyntax(Syntax & /*syntax*/, ActionFactory & /*action_factory*/)
{
}
