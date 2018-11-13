#ifndef CONFIG_HPP_INCL
#define CONFIG_HPP_INCL

#ifdef CHPP_INCLUDE_ALL
#define CHPP_INCLUDE_STD
#define CHPP_INCLUDE_SFML
#endif // CHPP_INCLUDE_ALL

#ifdef CHPP_INCLUDE_STD

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <string>
#include <vector>
using namespace std;

#endif // std
#ifdef CHPP_INCLUDE_SFML

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
using namespace sf;

#endif
#endif // CONFIG_HPP_INCL

