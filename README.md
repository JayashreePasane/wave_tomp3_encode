# wave_tomp3_encode
This is a simple application which encodes audio files (.WAVE format) to .MP3 format

WAV to MP3 Encoder
---------------------------------------------
- using lame static library based on LAME 3.100(installation required - http://lame.sourceforge.net/)
- supports encoding multiple files using pthread by providing input directory path
- Suppors Windows and Linux operating systems

## Build
- Linux: make
- Windows,mingw-32: make 

## Usage on Linux
```sh
waveToMp3Encode <input_directory>
```

## Usage on Windows
```cmd
waveToMp3Encode <input_directory>
```
## Todo