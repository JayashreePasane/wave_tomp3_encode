/*
 * wave.cpp
 *
 *  Created on: 19-Aug-2021
 *      Author: vishr
 */

#include "wave.h"
#include <iostream>

using namespace std;
using namespace filesystem;

namespace wave2mp3 {

vector<path> traverseDirectory(path const &directory) {
	vector<path> entries;

	for (auto entry : directory_iterator(directory)) {
		entries.push_back(entry);
	}

	return entries;
}

bool isWaveExtension(path const &filename) {

	auto extension = path::string_type { filename.extension() };
	transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

	return extension == path { ".wav" };
}

vector<path> getInput(path const &directory) {
	vector<path> files;

	for (auto entry : traverseDirectory(directory))
		if (isWaveExtension(entry)) {
			files.push_back(entry);
		}
	return files;
}

}
