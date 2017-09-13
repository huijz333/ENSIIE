package com.rpereira.ga.test;

import java.util.Random;

import com.rpereira.ga.Gene;

public class GeneBit extends Gene<Boolean> {

	public GeneBit(Boolean value) {
		super(value);
	}

	@Override
	public void mutate(Random rng) {
		super.set(!super.get());
	}

}
