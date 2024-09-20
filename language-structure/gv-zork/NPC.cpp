#include "Assets.h"
#include <iostream>
#include <string>
#include <vector>

/***************************************
 * NPC Class Definitions
 *
 * Authors:
 * 	Keri Lamberjack
 * 	Keagen Talsma 
 **************************************/

NPC::NPC(std::string name, std::string desc) {
	if (name.empty())
		throw std::invalid_argument("Name cannot be blank.");

	if (desc.empty()) 
		throw std::invalid_argument("Description cannot be blank.");

	this->name = name;
	this->desc = desc;	
	this->message_num = 0;
	this->has_gift = true;
}

std::string NPC::get_name() const {
	return this->name;
}

std::string NPC::get_desc() const {
	return this->desc;
}

std::string NPC::get_message() {
	if(!this->messages.size()) {
		return "They have nothing to say";
	}

	std::string message = this->messages[this->message_num];
	this->message_num = (this->message_num + 1) % this->messages.size();
	return message;
}

void NPC::add_message(std::string message) {
	this->messages.push_back(message);
}

bool NPC::get_has_gift() {
	return this->has_gift;
}

void NPC::take_gift() {
	this->has_gift = false;
}

std::ostream& operator<< (std::ostream& out, const NPC& npc) {
	out << npc.name;
	return out;
}

