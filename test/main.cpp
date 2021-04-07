/*
 * main.cpp
 *
 *  Created on: 7 Apr 2021
 *      Author: mathieu
 */

#include <iostream>

#include "../engine/ue_engine.hpp"

int main(void)
{
	ue::Real a = {10};
	ue::Real b = {34};
	ue::Real c = {0};

	c = a + b;

	std::cout << c.val << std::endl;
}
