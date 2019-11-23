project('openav_artyfx', ['cpp'])

conf_data = configuration_data()
conf_data.set('version', '1.3.1')

add_project_arguments('-Wno-unused-variable', language : 'cpp')
add_project_arguments('-Wno-reorder', language : 'cpp')
add_project_arguments('-Wno-sign-compare', language : 'cpp')
add_project_arguments('-fPIC', language : 'cpp')
add_project_arguments('-shared', language : 'cpp')

subdir('src')

dep_names = [
  'lv2',
  'cairo',
  'sndfile',
  'samplerate',
  'threads',
  'x11'
  ]
deps = []

foreach dep : dep_names
  deps += dependency(dep)
endforeach

install_subdir('artyfx.lv2', install_dir : 'lib/lv2/') 

artyfx_dsp = library('artyfx',
    [artyfx_dsp_src],
    install : true,
    name_prefix : '', # backwards compatible with old build system
    install_dir : 'lib/lv2/artyfx.lv2/',
    dependencies: deps)

# compile the benchmark
#executable('artyfx_benchmark', ' ,dependencies: artyfx_dsp)
