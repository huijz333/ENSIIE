package com.rpereira.ga;

import java.util.Random;

public abstract class Individual {

	private final Gene<?>[] genes;

	public Individual(int geneCount) {
		this.genes = new Gene[geneCount];
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
	public abstract double fitness();

	/** randomize genes of this instance */
	public abstract void randomize(Random rng);

	public static final Individual random(Class<? extends Individual> individualClass, Random rng) {
		try {
			Individual individual = individualClass.newInstance();
			individual.randomize(rng);
			return (individual);
		} catch (Exception e) {
			System.err.println(
					"Individual.random(...) : Instanciation error on individual class " + e.getLocalizedMessage());
			return (null);
		}
	}

	public abstract Individual reproduce(Individual female);

	public final void mutate(Random rng) {
		this.genes[rng.nextInt(this.getGenesCount())].mutate(rng);
	}
}
