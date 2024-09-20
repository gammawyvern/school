#include "Assets.h"
#include <iostream>
#include <vector>
#include <string>

//This is the real man's way to do C++
#include <memory>

/***************************************
 * Location Class Definitions
 *
 * Authors:
 * 	Keri Lamberjack
 * 	Keagen Talsma 
 **************************************/

Location::Location(std::string name, std::string desc) {
	if (name.empty())
		throw std::invalid_argument("Description cannot be blank.");

	if (desc.empty())
		throw std::invalid_argument("Description cannot be blank.");

	this->name = name;
	this->desc = desc;
	this->visited = false;
}

void Location::add_item(Item item) {
	this->items.push_back(item);
}

void Location::add_npc(NPC npc) {
	this->npcs.push_back(npc);
}

void Location::add_location(std::string direction, std::shared_ptr<Location> location) {
	if(direction.empty())
		throw std::invalid_argument("Direction cannot be blank.");

	if(this->neighbors.find(direction) != this->neighbors.end())
		throw std::invalid_argument("Direction already exists");

	this->neighbors[direction] = location.get();
}

void Location::set_visited() {
	this->visited = true;
}

std::vector<Item>& Location::get_items() {
	return this->items;
}

std::vector<NPC>& Location::get_npcs() {
	return this->npcs;
}

std::map<std::string, Location*>& Location::get_locations() {
	return this->neighbors;
}

std::string Location::get_name() {
	return this->name;
}

bool Location::get_visited() {
	return this->visited;
}

std::ostream& operator<<(std::ostream& out, const Location& location) {
	out << location.name << " - " << location.desc << std::endl; 
	// NPCs
	out << std::endl << "You see the following NPCs:" << std::endl;
	for(NPC npc: location.npcs)
		out << "\t- " << npc << std::endl;
	out << std::endl;
	// Items
	out << "You see the following Items:" << std::endl;
	for(Item item: location.items)
		out << "\t- " << item << std::endl; 
	out << std::endl;
	// Locations
	out << "You can go in the following directions:" << std::endl;
	for(std::pair<std::string, Location*> dir: location.neighbors) {
		out << "\t- " << dir.first << " - "; 
		if(dir.second->get_visited()) {
			out << dir.second->name << " (Visited)" << std::endl;
		} else {
			out << "Unknown" << std::endl;
		}
	}

	return out;
}

