project('healthreporter', 'cpp', version: '0.1.1')

sdbusplus_dep = dependency('sdbusplus')
pdi_dep = dependency('phosphor-dbus-interfaces')

executable('healthreporter',
    'main.cpp',
    dependencies: [sdbusplus_dep, pdi_dep],
    install: true)
