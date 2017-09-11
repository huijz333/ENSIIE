package com.rpereira.ga;

import java.util.Random;

public abstract class Gene<T> {

	private T value;

	public Gene(T value) {
		this.value = value;
	}

	public Gene() {
		this(null);
	}

	public final void set(T value) {
		this.value = value;
	}

	public final T get() {
		return (this.value);
	}

	public abstract void mutate(Random rng);
}
