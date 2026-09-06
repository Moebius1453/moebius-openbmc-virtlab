project('meminfo', 'cpp', version: '0.1.0')

sdbusplus_dep = dependency('sdbusplus')
sdbuspp = find_program('sdbus++', native: true)

yaml_root = meson.current_source_dir() / 'interfaces/yaml'

subdir('gen')

executable('meminfo',
    'main.cpp',
    gen_server_hpp,
    gen_server_cpp,
    dependencies: [sdbusplus_dep],
    include_directories: include_directories('.'),
    install: true)

