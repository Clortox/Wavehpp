# Wavehpp
Simple wrapper for PlaySound() in the winodws.h library

This is my first time trying to create something like this outside of class

Use with care :)

***

## Usage
### Public Methods

- wave::wave()
  - defualt ctor (not recomended for usage)
  
- wave::wave(std::string&)
  - takes path to .wav file to load

- wave::wave(const wave&)
  - copy ctor

- wave::~wave()
  - destructor

- wave& wave::operator=(wave&)
  - asignment operator

- wave::swap(wave&)
  - swap

- bool wave::isOk() const
  - check if the wav file was loaded properly and is ready to be played

- wave::play(bool)
  - play the loaded file
  - Note: passing false will pause execution till the file has finished playing
  - However, passing true will let execution continue normally will the sound plays

- wave::resign(std::string&)
  - loads a new file into memory to play from the provided path

- std::string wave::dumpbuffer() const
  - will output the file buffer

***

### Private Methods 

- bool wave::isWave(std::string&)
  - returns true if the magic bytes at the beggining of the file indicate a wav file (RIFF....WAVE)

***

### Private Values

- wave::buffer
  - char* ; points to the buffer where the file is stored on the heap
  
- wave::HInstance
  - HINSTANCE ; used by windows to tell different windows and similar processes apart

- wave::isok
  - bool ; stores weather or not the file is ready to be played

- wave::buffsize
  - int ; size of the file buffer
