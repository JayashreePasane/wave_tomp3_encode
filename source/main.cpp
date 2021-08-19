#include "wave.h"

#include <iostream>
#include <vector>

using namespace std;

/**
 * @brief	  Function prompts message on IO to provide information about exe Usage
 * @param[in] void
 * @return    void
 *
 */
void usage() {
	cout << "Usage Example:\n"
			"waveToMp3Encode input_wave_directory_path";
	exit(0);
}

int main(int argc, char *argv[]) {

	/* validate input provided */
	if (argc != 2) {
		cout << "ERROR: Too few arguments. See below usage:\n";
		usage();
	}

	/*get Directory input path*/
	const auto directory_path = std::string(argv[1]);

	/* process WaveFiles directory to get input files*/
	const auto &files = wave2mp3::getInput(path{directory_path});

	if (files.empty()) {
		cout << "no valid wave files found";
	} else {
		//wave2mp3();
	}

	return 0;
}

