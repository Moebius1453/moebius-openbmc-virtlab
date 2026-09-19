project('meminfoweb', 'cpp', version: '0.1.0')

sdbusplus_dep = dependency('sdbusplus')
pdi_dep = dependency('phosphor-dbus-interfaces')

executable('meminfoweb',
    'main.cpp',
    dependencies: [sdbusplus_dep, pdi_dep],
    install: true)
