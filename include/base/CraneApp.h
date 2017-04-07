#ifndef CRANEAPP_H
#define CRANEAPP_H

#include "MooseApp.h"

class CraneApp;

template<>
InputParameters validParams<CraneApp>();

class CraneApp : public MooseApp
{
public:
  CraneApp(InputParameters parameters);
  virtual ~CraneApp();

  static void registerApps();
  static void registerObjects(Factory & factory);
  static void associateSyntax(Syntax & syntax, ActionFactory & action_factory);
};

#endif /* CRANEAPP_H */
