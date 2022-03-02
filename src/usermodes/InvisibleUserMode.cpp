#include "InvisibleUserMode.hpp"

InvisibleUserMode::InvisibleUserMode(void)
	: AUserMode('i', AUserMode::canSet | AUserMode::canUnset)
{}

InvisibleUserMode::~InvisibleUserMode(void)
{}

