package com.rpereira.ga.test;

import com.rpereira.ga.Generation;

public class Main {

	public static void main(String[] args) {
		Generation generation = new Generation();
		generation.random(IndividualSumBits.class, 1000);
		
		System.out.println(generation.mediumFitness() + " : " + generation.medianFitness() + " : "
				+ generation.bestIndividual().fitness());
		for (int i = 0; i < 100; i++) {
			generation = generation.nextGeneration();
			System.out.println(generation.mediumFitness() + " : " + generation.medianFitness() + " : "
					+ generation.bestIndividual().fitness());
		}

		for (int i = 0; i < 10; i++) {
			System.out.println(generation.get(i).fitness());
		}
	}
}
