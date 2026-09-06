gen_server_hpp = custom_target(
    'memorystats-server-header',
    input: yaml_root / 'xyz/moebius/MemoryStats.interface.yaml',
    output: 'server.hpp',
    command: [
        sdbuspp, '-r', yaml_root,
        'interface', 'server-header', 'xyz/moebius/MemoryStats',
    ],
    capture: true,
)

gen_server_cpp = custom_target(
    'memorystats-server-cpp',
    input: yaml_root / 'xyz/moebius/MemoryStats.interface.yaml',
    output: 'server.cpp',
    command: [
        sdbuspp, '-r', yaml_root,
        'interface', 'server-cpp', 'xyz/moebius/MemoryStats',
    ],
    capture: true,
)
