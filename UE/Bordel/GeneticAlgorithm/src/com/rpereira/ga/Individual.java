package com.rpereira.ga;

import java.util.Random;

public abstract class Individual {

	private final Gene<?>[] genes;
	private double fitness;

	public Individual(int geneCount) {
		this.genes = new Gene[geneCount];
		this.fitness = 0;
	}

	public final Gene<?> getGene(int index) {
		return (this.genes[index]);
	}

	public final void setGene(Gene<?> gene, int index) {
		if (index < 0 || index >= this.genes.length) {
			System.err.println("Individual.setGene(...) out of bounds");
			return;
		}
		this.genes[index] = gene;
	}

	public final int getGenesCount() {
		return (this.genes.length);
	}

	/** return the fitness of this individual (the greater, the better) */
	public final double fitness() {
		return (this.fitness);
	}

	public final void updateFitness() {
		this.fitness = this.calculateFitness();
	}

	protected abstract double calculateFitness();

	/** randomize genes of this instance */
	public abstract void randomize(Random rng);

	public static final Individual random(Class<? extends Individual> individualClass, Random rng) {
		try {
			Individual individual = individualClass.newInstance();
			individual.randomize(rng);
			individual.updateFitness();
			return (individual);
		} catch (Exception e) {
			System.err.println(
					"Individual.random(...) : Instanciation error on individual class " + e.getLocalizedMessage());
			return (null);
		}
	}

	public final Individual cross(Individual... individuals) {
		Individual child = this.crossIndividuals(individuals);
		child.updateFitness();
		return (child);
	}

	protected abstract Individual crossIndividuals(Individual... individuals);

	public final void mutate(Random rng) {
		this.genes[rng.nextInt(this.getGenesCount())].mutate(rng);
	}
}
