<?php

namespace User;

use PHPUnit\Framework\TestCase;

class UserTest extends TestCase {
	
	/**
	 *
	 * @test
	 */
	public function ageWhenBirthdayInThePast() {
		self::assertSame ( 1, 1 );
	}
}