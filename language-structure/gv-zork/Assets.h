#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <functional>

//This is the real man's way to do C++
#include <memory>

class Item;
class NPC;
class Location;
class Game;

typedef std::function<void(Game*, std::vector<std::string>)> command;

/***************************************
 * Class Declarations
 *
 * Authors:
 * 	Keri Lamberjack
 * 	Keagen Talsma 
 **************************************/



/***********************************************************
 * Item Class
 *
 * Holds a name, description, calorie amount, and weight.
 * Put into locations with Location::add_item().
 * Items are passed around by copy.
 **********************************************************/

class Item {
	public:
		/***********************************************
		 * Args:
		 * 	name: Item name string.
		 * 	desc: Item description string.
		 * 	calories: Amount of calories provided
		 * 		to elf (if given to elf).
		 * 	weight: Amount of weight in player's
		 * 		inventory.
		 *
		 * Throws:
		 * 	invalid_argument:
		 * 		name is empty.
		 * 		desc is empty.
		 * 		calories is negative or >1000.
		 * 		calories is negative or >500.
		 **********************************************/
		Item(std::string name, std::string desc, int calories, float weight);

		std::string get_name();
		float get_weight();
		int get_calories();

		/***********************************************
		 * Returns: 
		 * 	NAME (X Calories) - X lb - DESCRIPTION
		 **********************************************/
		friend std::ostream& operator<<(std::ostream& out, const Item& item);
	private:
		std::string name;
		std::string desc;
		int calories;
		float weight;
};



/***********************************************************
 * NPC Class
 *
 * Holds a name and description, messages, and a gift.
 * NPCs are passed around by copy.
 **********************************************************/

class NPC {
	public:
		/***********************************************
		 * Args:
		 * 	name: NPC name string.
		 * 	desc: NPC description string.
		 *
		 * Throws:
		 * 	invalid_argument:
		 * 		name is empty.
		 * 		desc is empty.
		 **********************************************/
		NPC(std::string name, std::string desc);

		/***********************************************
		 * Args:
		 * 	message: Message to add in NPC message
		 * 		rotation.
		 **********************************************/
		void add_message(std::string message);

		/***********************************************
		 * Returns:
		 * 	The message at the current message
		 * 	index. Then increments message_num to
		 * 	set current message to next message. 
		 **********************************************/
		std::string get_message();

		/***********************************************
		 * Sets has_gift to false.
		 **********************************************/
		void take_gift();

		bool get_has_gift();
		std::string get_name() const;
		std::string get_desc() const;

		/***********************************************
		 * Returns:
		 * 	NAME
		 **********************************************/
		friend std::ostream& operator<<(std::ostream& out, const NPC& npc);
	private:
		std::string name;
		std::string desc;
		int message_num;
		std::vector<std::string> messages;
		bool has_gift;
};



/***********************************************************
 * Location Class
 *
 * Holds a name and description, vector of Items and NPCs,
 * as well as a map of connected locations, and a visited
 * flag.
 **********************************************************/

class Location {
	public:
		/***********************************************
		 * Thows:
		 * 	invalid_argument:
		 * 		name is empty.
		 * 		desc is empty.
		 **********************************************/
		Location(std::string name, std::string desc);

		void add_item(Item item);
		void add_npc(NPC npc);

		/***********************************************
		 * Sets the value at the key in neighbors map
		 * to point to another location.
		 *
		 * Args:
		 * 	direction: key (string) to insert
		 * 		other location pointer 
		 * 	location: pointer to other location
		 * 		connected by the direction
		 *
		 * Thows:
		 * 	invalid_argument:
		 * 		direction is empty.
		 * 		direction key already exists
		 * 			in neighbors
		 **********************************************/
		void add_location(std::string direction, std::shared_ptr<Location> location);
		void set_visited();
		std::vector<Item>& get_items();
		std::vector<NPC>& get_npcs();
		std::map<std::string, Location*>& get_locations();
		std::string get_name();
		bool get_visited();

		/***********************************************
		 * Returns:
		 *	NAME - DESC
		 *
		 *	You see the following NPCs:
		 *		ALL NPCS
		 *
		 *	You see the following Items:
		 *		ALL ITEMS
		 *
		 *	You can go in the following Directions:
		 *		ALL LOCATIONS
		 *		- DIR - NAME (Visited)
		 *		- DIR - Unkown
		 **********************************************/
		friend std::ostream& operator<<(std::ostream& out, const Location& location);
	private:
		std::string name;
		std::string desc;
		bool visited;
		std::vector<NPC> npcs;
		std::vector<Item> items;
		std::map<std::string, Location*> neighbors;
};

class Game {
	public:
		/***********************************************
		 * Sets up entire game. Creates commands,
		 * world, and sets current location to a 
		 * random one.
		 **********************************************/
		Game();

		/***********************************************
		 * Sets up all locations, including their items
		 * and npcs. Adds all locations to locations
		 * vector.
		 **********************************************/
		void create_world();

		/***********************************************
		 * Returns:
		 * 	Map of command strings to command
		 * 	functions.
		 **********************************************/
		std::map<std::string, command> setup_commands();

		/***********************************************
		 * Returns:
		 * 	Pointer to a random location in the
		 * 	current locations vector.
		 **********************************************/
		Location* random_location();

		/***********************************************
		 * Main gameplay loop. Gets and interprets
		 * user input to call command functions. Ends
		 * when elf is satiated or player quits.
		 **********************************************/
		void play();

		/***********************************************
		 * Gets user input. Wipes tokens vector, then
		 * sets the first word as the command, and
		 * pushes the rest onto the tokens vector.
		 *
		 * Args:
		 * 	tokens: vector to put each additional
		 * 		word from user input into.
		 *
		 * Return:
		 * 	command: command string that represents
		 * 		the first user input word
		 **********************************************/
		std::string get_input(std::vector<std::string>& tokens);

		/***********************************************
		 * Prints out basic help
		 **********************************************/
		void print_help(std::vector<std::string> tokens);
		/***********************************************
		 * Prints message from npc in tokens
		 **********************************************/
		void talk(std::vector<std::string> tokens);
		/***********************************************
		 * Prints description from npc in tokens
		 **********************************************/
		void meet(std::vector<std::string> tokens);
		/***********************************************
		 * Moves item in tokens from location
		 * into player_inventory
		 **********************************************/
		void take(std::vector<std::string> tokens);
		/***********************************************
		 * Moves item in tokens from players inventory
		 * to location. If at the ravines, feeds elf
		 * instead.
		 **********************************************/
		void give(std::vector<std::string> tokens);
		/***********************************************
		 * Moves to location at first direction in
		 * tokens 
		 **********************************************/
		void go(std::vector<std::string> tokens);
		/***********************************************
		 * Print's out the player's inventory 
		 **********************************************/
		void show_items(std::vector<std::string> tokens);
		/***********************************************
		 * Reprints the player's current location info 
		 **********************************************/
		void look(std::vector<std::string> tokens);
		/***********************************************
		 * Quits out of game, causing the player to lose
		 **********************************************/
		void quit(std::vector<std::string> tokens);
		/***********************************************
		 * Gets a random gift from NPC if they have one
		 **********************************************/
		void smile(std::vector<std::string> tokens);
		/***********************************************
		 * Removes item from player inventory. Does
		 * not add to location. (Should have renamed to 
		 * destroy or something, but I was too lazy)
		 **********************************************/
		void drop(std::vector<std::string> tokens);
	private:
		/***********************************************
		 * Returns:
		 * 	A random gift (currently out of 3 gifts)
		 **********************************************/
		Item get_random_gift();

		/***********************************************
		 * Attempts to add item to player inventory.
		 * Fails if weight would go over 30.
		 *
		 * Args:
		 * 	item: Item to put into player inventory
		 *
		 * Return:
		 * 	bool: If item was added to player
		 * 		inventory 
		 **********************************************/
		bool give_player_item(Item item);

		/***********************************************
		 * If item is edible, reduces elf hunger by
		 * calorie amount of item. If item is not 
		 * edible, teleports player to random location. 
		 **********************************************/
		void feed_elf(Item item);

		std::map<std::string, command> commands;
		std::vector<Item> player_inventory;
		float player_weight;
		std::vector<std::shared_ptr<Location> > locations;
		Location* player_location;
		int elf_hunger;
		bool game_in_progress;
};

