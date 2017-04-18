[GlobalParams]
  # elementary charge in coulombs
  # Boltzmann constant in J/K
  # Permittivity of free space in F/m
  q = 1.6021766208e-19
  k = 1.38064852e-23
  epsilon_0 = 8.854187817e-12
[]

[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 20
  xmax = 0.001
  ymax = 0.001
  elem_type = QUAD
[]

[Variables]
  [./T]
    initial_condition = 300.
    scaling = 0.001
  [../]
  [./psi]
    initial_condition = 0.5
  [../]
  [./n_tilde]
    initial_condition = 22.8839
  [../]
  [./p_tilde]
    initial_condition = 22.8839
  [../]
[]

[Kernels]
  [./psi_space_charge]
    type = SpaceCharge
    n_tilde = n_tilde
    p_tilde = p_tilde
    temperature = T
    semiconductor = Si
    variable = psi
  [../]
  [./p_carrier_transport]
    type = CarrierTransport
    carrier_type = hole
    statistics = Boltzmann
    potential = psi
    temperature = T
    semiconductor = Si
    variable = p_tilde
  [../]
  [./temperature_diff]
    type = Diffusion
    variable = T
  [../]
  [./psi_diffusion]
    type = ScaledDiffusion
    semiconductor = Si
    variable = psi
  [../]
  [./n_tilde_carrier_transport]
    type = CarrierTransport
    carrier_type = electron
    statistics = Boltzmann
    potential = psi
    temperature = T
    semiconductor = Si
    variable = n_tilde
  [../]
  [./n_tilde_time]
    type = TimeDerivative
    variable = n_tilde
  [../]
  [./p_tilde_time]
    type = TimeDerivative
    variable = p_tilde
  [../]
[]

[BCs]
  [./ohmic_psi_left]
    type = OhmicContact
    statistics = Boltzmann
    contact_element = potential
    temperature = T
    potential = 1
    semiconductor = Si
    boundary = left
    variable = psi
  [../]
  [./ohmic_psi_right]
    type = OhmicContact
    statistics = Boltzmann
    contact_element = potential
    temperature = T
    potential = 0
    semiconductor = Si
    boundary = right
    variable = psi
  [../]
  [./ohmic_n_left]
    type = OhmicContact
    statistics = Boltzmann
    contact_element = electron
    temperature = T
    potential = 1
    semiconductor = Si
    boundary = left
    variable = n_tilde
  [../]
  [./ohmic_n_right]
    type = OhmicContact
    statistics = Boltzmann
    contact_element = electron
    temperature = T
    potential = 0
    semiconductor = Si
    boundary = right
    variable = n_tilde
  [../]
  [./ohmic_p_left]
    type = OhmicContact
    statistics = Boltzmann
    contact_element = hole
    temperature = T
    potential = 1
    semiconductor = Si
    boundary = left
    variable = p_tilde
  [../]
  [./ohmic_p_right]
    type = OhmicContact
    statistics = Boltzmann
    contact_element = hole
    temperature = T
    potential = 0
    semiconductor = Si
    boundary = right
    variable = p_tilde
  [../]
  [./temperature_left]
    type = DirichletBC
    variable = T
    boundary = left
    value = 300
  [../]
  [./temperature_right]
    type = DirichletBC
    variable = T
    boundary = right
    value = 300
  [../]
  [./carrier_bc_n]
    type = CarrierTransportBC
    boundary = 'left right top bottom'
    carrier_type = electron
    statistics = Boltzmann
    potential = psi
    temperature = T
    semiconductor = Si
    variable = n_tilde
  [../]
  [./carrier_bc_p]
    type = CarrierTransportBC
    boundary = 'left right top bottom'
    carrier_type = hole
    statistics = Boltzmann
    potential = psi
    temperature = T
    semiconductor = Si
    variable = p_tilde
  [../]
[]

[Materials]
  [./Si]
    type = SemiconductorMaterial
    semiconductor = Si
    temperature = T
  [../]
[]

[UserObjects]
  [./Si]
    type = SemiconductorUserObject
    statistics = Boltzmann
    Eg = 1.17
    alpha = -4.73e-4
    beta = 636
    Nc = 6.2e15
    Nv = 3.6e15
    electron_mobility = 0.1
    hole_mobility = 0.045
  [../]
[]

[Preconditioning]
  [./SMP]
    type = SMP
    solve_type = PJFNK
    petsc_options_iname = '-pc_type -pc_hypre_type -ksp_gmres_restart'
    petsc_options_value = 'hypre boomeramg 101'
  [../]
[]

[Executioner]
  type = Transient
  dt = 1e-6
  trans_ss_check = true
  nl_abs_tol = 1e-12
  l_max_its = 100
  ss_check_tol = 1e-5
[]

[Outputs]
  [./Exodus]
    type = Exodus
    output_material_properties = true
  [../]
[]
