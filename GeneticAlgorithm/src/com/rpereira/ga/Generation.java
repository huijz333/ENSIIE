package com.rpereira.ga;

import java.util.ArrayList;
import java.util.Random;

public class Generation {

	private final ArrayList<Individual> individuals;
	private final Random rng;

	public Generation() {
		this(new Random());
	}

	public Generation(Random rng) {
		this.individuals = new ArrayList<Individual>();
		this.rng = rng;
	}

	public final void add(Individual individual) {
		int index;
		for (index = 0; index < this.individuals.size(); index++) {
			if (individual.fitness() > this.individuals.get(index).fitness()) {
				break;
			}
		}
		this.individuals.add(index, individual);
	}

	public final void remove(Individual individual) {
		this.individuals.remove(individual);
	}

	public final Individual get(int index) {
		return (this.individuals.get(index));
	}

	public final double fitness() {
		double fitness = 0;
		for (Individual i : this.individuals) {
			fitness += i.fitness();
		}
		return (fitness);
	}

	public final double mediumFitness() {
		return (this.fitness() / (double) this.individuals.size());
	}

	public final double medianFitness() {
		return (this.individuals.get(this.individuals.size() / 2).fitness());
	}

	public final Generation nextGeneration() {
		Generation nextGen = new Generation();

		Individual male, female, child;
		int mid = this.individuals.size() / 2;
		int size = this.individuals.size() * 2;
		for (int i = 0; i < size; i++) {
			male = this.get(this.rng.nextInt(mid + 1));
			female = this.get(this.rng.nextInt(mid + 1));
			child = male.cross(male, female);
			nextGen.add(child);
		}
		nextGen.half();
		return (nextGen);
	}

	/** half the generation: the half less fit inviduals are removed */
	private final void half() {
		int toRemove = this.individuals.size() / 2;
		for (int i = 0; i < toRemove; i++) {
			this.individuals.remove(this.individuals.size() - 1);
		}
	}

	/**
	 * add the given number of random individuals to the generation
	 * 
	 * @param individuals
	 * @param count
	 */
	public final void random(Class<? extends Individual> individualClass, int number) {
		for (int i = 0; i < number; i++) {
			this.add(Individual.random(individualClass, this.rng));
		}
	}

	public final Individual bestIndividual() {
		if (this.individuals.size() == 0) {
			System.err.println("Generation.bestIndividual(...) : empty generation (no individual)");
			return (null);
		}
		Individual best = this.individuals.get(0);
		double bestFitness = best.fitness();
		for (int i = 1; i < this.individuals.size(); i++) {
			Individual other = this.individuals.get(i);
			double otherFitness = other.fitness();
			if (bestFitness < otherFitness) {
				bestFitness = otherFitness;
				best = other;
			}
		}
		return (best);
	}
}
