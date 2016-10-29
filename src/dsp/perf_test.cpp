#include "dsp_reverb.hxx"
#include "dsp_bitcrusher.hxx"
#include "dsp_compander.hxx"

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

//BENCHMARK(reverb    )->RangeMultiplier(2)->Range(8, 1024);
BENCHMARK(reverb    )->Range(8, 1024);
BENCHMARK(bitcrusher)->Range(8, 1024);
BENCHMARK(compander)->Range(8, 1024);

BENCHMARK_MAIN()
