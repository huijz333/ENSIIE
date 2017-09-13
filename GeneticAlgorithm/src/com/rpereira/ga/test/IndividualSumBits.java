package com.rpereira.ga.test;

import java.util.Random;

import com.rpereira.ga.Individual;

public class IndividualSumBits extends Individual {

	public IndividualSumBits() {
		super(64);
	}

	@Override
	public double calculateFitness() {
		double fitness = 0;
		for (int i = 0; i < super.getGenesCount(); i++) {
			fitness += ((Boolean) super.getGene(i).get()) ? 1 : 0;
		}
		return (fitness);
	}

	@Override
	public void randomize(Random rng) {
		for (int i = 0; i < super.getGenesCount(); i++) {
			super.setGene(new GeneBit(new Boolean(rng.nextInt() % 2 == 0)), i);
		}
	}

	@Override
	protected Individual crossIndividuals(Individual... individuals) {
		Individual male = individuals[0];
		Individual female = individuals[1];
		IndividualSumBits child = new IndividualSumBits();
		int mid = this.getGenesCount() / 2;
		int i = 0;
		for (; i < mid; i++) {
			child.setGene(male.getGene(i), i);
		}
		for (; i < this.getGenesCount(); i++) {
			child.setGene(female.getGene(i), i);
		}
		return (child);
	}
}
