#include "dsp_reverb.hxx"
#include "dsp_bitcrusher.hxx"
#include "dsp_compander.hxx"
#include "dsp_widener.hxx"
#include "dsp_delay.hxx"
#include "dsp_masher.hxx"
#include "dsp_filters.hxx"
#include "dsp_filters_svf.hxx"
#include "dsp_wah.hxx"

#include "benchmark/benchmark.h"

#define SMPS  44100

float inL[SMPS];
float outL[SMPS];
float inR[SMPS];
float outR[SMPS];

float *ins[]  = {inL, inR};
float *outs[] = {outL, outR};

void reverb(benchmark::State& state)
{
	Reverb r(SMPS);
	const uint32_t nf = state.range(0);
	if(nf > SMPS)
		return;
	r.dryWet(0.5);

	while (state.KeepRunning()) {
		r.process(nf, ins, outs);
	}
}

void bitcrusher(benchmark::State& state)
{
	BitCrusher b(SMPS);
	b.active(1);

	const uint32_t nf = state.range(0);
	if(nf > SMPS)
		return;

	while (state.KeepRunning()) {
		b.process(nf, inL, outL);
	}
}

void compander(benchmark::State& state)
{
	Compander c(SMPS);
	c.active(1);

	const uint32_t nf = state.range(0);
	if(nf > SMPS)
		return;

	while (state.KeepRunning()) {
		c.process(nf, inL, outL);
	}
}

void widener(benchmark::State& state)
{
	Widener w(SMPS);

	const uint32_t nf = state.range(0);
	if(nf > SMPS)
		return;

	while (state.KeepRunning()) {
		w.process(nf, inL, outL, inR, outR);
	}
}

void filters(benchmark::State& state)
{
	Filters f(SMPS);

	const uint32_t nf = state.range(0);
	if(nf > SMPS)
		return;

	while (state.KeepRunning()) {
		f.process(nf, ins, outs);
	}
}

void filter_svf(benchmark::State& state)
{
	FiltersSVF f(SMPS);

	const uint32_t nf = state.range(0);
	if(nf > SMPS)
		return;

	while (state.KeepRunning()) {
		f.process(nf, inL, outL);
	}
}

void filter_stereo_svf(benchmark::State& state)
{
	FiltersSVF fl(SMPS);
	FiltersSVF fr(SMPS);

	const uint32_t nf = state.range(0);
	if(nf > SMPS)
		return;

	while (state.KeepRunning()) {
		fl.process(nf, inL, outL);
		fr.process(nf, inR, outR);
	}
}


void delay(benchmark::State& state)
{
	Delay d(SMPS);

	const uint32_t nf = state.range(0);
	if(nf > SMPS)
		return;

	while (state.KeepRunning()) {
		d.process(nf, inL, outL);
	}
}

void masher(benchmark::State& state)
{
	Masher m(SMPS);

	const uint32_t nf = state.range(0);
	if(nf > SMPS)
		return;

	while (state.KeepRunning()) {
		m.process(nf, inL, outL);
	}
}


void wah(benchmark::State& state)
{
	Wah w(SMPS);

	const uint32_t nf = state.range(0);
	if(nf > SMPS)
		return;

	while (state.KeepRunning()) {
		w.process(nf, inL, outL);
	}
}


BENCHMARK(reverb    )->Range(8, 1024);
BENCHMARK(bitcrusher)->Range(8, 1024);
BENCHMARK(compander )->Range(8, 1024);
BENCHMARK(widener   )->Range(8, 1024);
BENCHMARK(delay     )->Range(8, 1024);
BENCHMARK(masher    )->Range(8, 1024);
BENCHMARK(filters   )->Range(8, 1024);
BENCHMARK(filter_svf)->Range(8, 1024);
BENCHMARK(filter_stereo_svf)->Range(8, 1024);
BENCHMARK(wah       )->Range(8, 1024);

BENCHMARK_MAIN()
