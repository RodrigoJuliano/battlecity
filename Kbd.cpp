#include "Kbd.h"

std::unordered_set<KbdKey> Kbd::pressed_keys;

bool Kbd::startedPressKey(KbdKey key) {
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

bool Kbd::isKeyPressed(KbdKey key) {
	return sf::Keyboard::isKeyPressed(key);
}
