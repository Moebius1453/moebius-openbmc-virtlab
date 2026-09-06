project('meminfo', 'cpp', version: '0.1.0')

sdbusplus_dep = dependency('sdbusplus')
sdbuspp = find_program('sdbus++', native: true)

yaml_root = meson.current_source_dir() / 'interfaces/yaml'

gen_server_hpp = custom_target(
    'memorystats-server-header',
    input: 'interfaces/yaml/xyz/moebius/MemoryStats.interface.yaml',
    output: 'xyz/moebius/MemoryStats/server.hpp',
    command: [
        sdbuspp, '-r', yaml_root,
        'interface', 'server-header', 'xyz/moebius/MemoryStats',
    ],
    capture: true,
)

gen_server_cpp = custom_target(
    'memorystats-server-cpp',
    input: 'interfaces/yaml/xyz/moebius/MemoryStats.interface.yaml',
    output: 'xyz/moebius/MemoryStats/server.cpp',
    command: [
        sdbuspp, '-r', yaml_root,
        'interface', 'server-cpp', 'xyz/moebius/MemoryStats',
    ],
    capture: true,
)

executable('meminfo',
    'main.cpp',
    gen_server_hpp,
    gen_server_cpp,
    dependencies: [sdbusplus_dep],
    include_directories: include_directories('.'),
    install: true)

