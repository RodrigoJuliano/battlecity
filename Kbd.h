#pragma once

#include <SFML/Window/Keyboard.hpp>
#include <unordered_set>

using KbdKey = sf::Keyboard::Key;

class Kbd {
private:
	inline static std::unordered_set<KbdKey> pressed_keys;
public:
	// Check if started to press a key
	// (returns true if the key is pressed and only returns
	// true again after releasing and pressing again)
	inline static bool startedPressKey(KbdKey key) {
		if (sf::Keyboard::isKeyPressed(key)) {
			if (pressed_keys.find(key) == pressed_keys.end()) {
				pressed_keys.emplace(key);
				return true;
			}
			return false;
		}
		pressed_keys.erase(key);
		return false;
	}
	// Check if a key is pressed
	inline static bool isKeyPressed(KbdKey key) {
		return sf::Keyboard::isKeyPressed(key);
	}
};