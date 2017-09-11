package com.rpereira.ga.test;

import java.util.Random;

import com.rpereira.ga.Gene;
import com.rpereira.ga.Individual;

public class IndividualSumBits extends Individual {

	public IndividualSumBits() {
		super(64);
	}

	@Override
	public double fitness() {
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
	public Individual reproduce(Individual female) {
		IndividualSumBits child = new IndividualSumBits();
		for (int i = 0; i < this.getGenesCount(); i++) {
			child.setGene(i % 2 == 0 ? this.getGene(i) : female.getGene(i), i);
		}
		return (child);
	}

}
