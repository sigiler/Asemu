
project('Asemu', 'cpp',
  default_options : ['cpp_std=c++17'])

src = [
    'src/core/board.cpp',
    'src/core/cpu.cpp',
    'src/core/keys.cpp',
    'src/core/mem.cpp',
    'src/core/ports.cpp',
    'src/core/screen.cpp',
    'src/core/speaker.cpp',
    'src/core/timer.cpp',
    'src/frontend/sdl_app.cpp',
    'src/frontend/sdl_input.cpp',
    'src/frontend/sdl_output.cpp',
    'src/libs/configfile.cpp',
    'src/libs/ringbuffer.cpp',
    'src/tests/test_hw.cpp',
    'src/tests/test_opcodes.cpp',
    'src/utils/logger.cpp',
    'src/utils/profiler.cpp',
    'src/utils/tester.cpp',
    'src/emu.cpp',
    'src/main.cpp',
]

dep = dependency('sdl2')

executable('Asemu', src, dependencies : dep)
