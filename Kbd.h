#pragma once

#include <SFML/Window/Keyboard.hpp>
#include <unordered_set>

using KbdKey = sf::Keyboard::Key;

class Kbd {
private:
	static std::unordered_set<KbdKey> pressed_keys;
public:
	// Check if started to press a key
	// (returns true if the key is pressed and only returns
	// true again after releasing and pressing again)
	static bool startedPressKey(KbdKey key);
	// Check if a key is pressed
	static bool isKeyPressed(KbdKey key);
};