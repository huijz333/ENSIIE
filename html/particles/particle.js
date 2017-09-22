
var Particle = function() {
	
	this.x = 0; //pos x
	this.y = 0; //pos y
	this.vx = 0; //velocity x
	this.vy = 0; //velocity y
	this.ax = 0; //acc x
	this.ay = 0; //acc y
	this.maxhp = 60; //max health point
	this.hp = this.maxhp; // hp
	this.r = 1.0; //red
	this.g = 0.0; //green
	this.b = 0.0; //blue
	this.a = 1.0; //alpha
	this.size = 16.0;

	this.getX = function() {
		return (this.x);
	}

	this.getY = function() {
		return (this.y);
	}

	this.setX = function(value) {
		this.x = value;
	}

	this.setY = function(value) {
		this.y = value;
	}

	this.setR = function(value) {
		this.r = value;
	}

	this.setG = function(value) {
		this.g = value;
	}

	this.setB = function(value) {
		this.b = value;
	}

	this.setA = function(value) {
		this.a = value;
	}

	this.getR = function() {
		return (this.r);
	}

	this.getG = function() {
		return (this.g);
	}

	this.getB = function() {
		return (this.b);
	}

	this.getA = function() {
		return (this.a);
	}

	this.getHealth = function() {
		return (this.hp);
	}

	this.getMaxHealth = function() {
		return (this.maxhp);
	}

	this.getHealthRatio = function() {
		return (this.hp / this.maxhp);
	}

	this.setHealth = function(health) {
		this.hp = health;
	}

	this.setMaxHealth = function(maxhealth) {
		this.maxhp = maxhealth;
	}

	this.getSize = function() {
		return (this.size);
	}

	this.setSize = function(value) {
		this.size = value;
	}

	this.isDead = function() {
		return (this.hp <= 0);
	}

	this.setDead = function () {
		this.hp = 0;
	}

	this.setVx = function(value) {
		this.vx = value;
	}

	this.setVy = function(value) {
		this.vy = value;
	}

	this.setAx = function(value) {
		this.ax = value;
	}

	this.setAy = function(value) {
		this.ay = value;
	}

	this.update = function(dt) {
		this.vx += this.ax * dt;
		this.vy += this.ay * dt;

		this.x += this.vx * dt;
		this.y += this.vy * dt;

		this.hp -= dt;
	}
}

var ParticleSource = function(n) {
	this.n = n;
	this.x = 0; //pos x
	this.y = 0; //pos y
	this.particles = [];

	for (var i = 0 ; i < N ; i++) {
		this.particles.push(new Particle());
	}

	this.getN = function() {
		return (this.n);
	}

	this.getX = function() {
		return (this.x);
	}

	this.getY = function() {
		return (this.y);
	}

	this.getParticle = function(i) {
		return (this.particles[i]);
	}

	this.setX = function(value) {
		this.x = value;
	}

	this.setY = function(value) {
		this.y = value;
	}
}
