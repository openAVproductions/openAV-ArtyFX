artyfx_dsp_src += files(
  'bitta.cxx',
  'artyfx.cxx',
  #'capta.cxx',
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
gbench = dependency('benchmark', required: false)
sndfile = dependency('sndfile', required: false)
pthreads = dependency('threads')

if gbench.found()
    executable('artyfx_benchmark', artyfx_dsp_src + ['perf_test.cpp'],
        dependencies: [gbench, sndfile, pthreads])
endif
