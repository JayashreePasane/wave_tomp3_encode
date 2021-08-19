/*
 * wave.h
 *
 *  Created on: 19-Aug-2021
 *      Author: vishr
 */

#ifndef WAVE_H_
#define WAVE_H_

#include <filesystem>
#include <fstream>
#include <vector>

using namespace std::filesystem::__cxx11;

namespace wave2mp3 {

	std::vector<path> getInput(path const& directory);
	bool isWaveExtension(path const& filename);
	std::vector<path> traverseDirectory(path const &directory);
}

#endif /* WAVE_H_ */
