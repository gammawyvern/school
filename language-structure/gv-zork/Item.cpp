#include "Assets.h"
#include <string>
#include <iostream>

/***************************************
 * Location Class Definitions
 *
 * Authors:
 * 	Keri Lamberjack
 * 	Keagen Talsma 
 **************************************/

Item::Item(std::string name, std::string desc, int calories, float weight) {
	if (name.empty())
		throw std::invalid_argument("Name cannot be blank.");
	
	if (desc.empty())
		throw std::invalid_argument("Description cannot be blank.");

	if (calories < 0 || calories > 1000)
		throw std::invalid_argument("Calories must be between 0 and 1000");

	if (weight < 0 || weight > 500)
		throw std::invalid_argument("Weight must be between 0 and 500");

	this->name = name;
	this->desc = desc;
	this->calories = calories;
	this->weight = weight;
}

std::string Item::get_name() {
	return this->name;
}

float Item::get_weight() {
	return this->weight;
}

int Item::get_calories() {
	return this->calories;
}

std::ostream& operator<< (std::ostream& out, const Item& item) {
	out << item.name << " (" << item.calories << " calories) - ";
	out << item.weight << " lb - " << item.desc;
	return out;
}

