package com.rpereira.ga.test;

import com.rpereira.ga.Generation;

public class Main {

	public static void main(String[] args) {
		Generation generation = new Generation();
		generation.random(IndividualSumBits.class, 1000);
		for (int i = 0; i < 10000; i++) {
			generation = generation.nextGeneration();
			System.out.println(generation.bestIndividual().fitness());
		}
	}
}
