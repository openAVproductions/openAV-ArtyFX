artyfx_dsp_src += files(
  'bitta.cxx',
  'artyfx.cxx',
  'capta.cxx',
  'driva.cxx',
  'filta.cxx',
  'masha.cxx',
  'ringbuffer.cxx',
  'satma.cxx',
  'whaaa.cxx',
  'bitta.cxx',
  'della.cxx',
  'ducka.cxx',
  'kuiza.cxx',
  'panda.cxx',
  'roomy.cxx',
  'vihda.cxx'
  )

subdir('eq')
subdir('rr')

# compile the benchmark
sndfile = dependency('sndfile')
pthreads = dependency('threads')

gbench = dependency('benchmark', required: false)

if gbench.found()
    gbench = dependency('benchmark')
    executable('artyfx_benchmark', artyfx_dsp_src + ['perf_test.cpp'],
        dependencies: [gbench, sndfile, pthreads])
endif
