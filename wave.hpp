//////////////////////////////////////////////////////////////////////////
//wave.hpp
//load a .wav file into memeory and play at will
//MUST ADD winmm TO YOUR LINKER SETTINGS ELSE THIS WILL NOT WORK
//
//Free to use, modify, publish, etc.
//if you're feeling nice, mention me :D
//
//////////////////////////////////////////////////////////
//Date: 11-5-19                                         //
//Author: Tyler Perkins                                 //
//Pseudonym: Clortox                                    //
//modified from the code provided by:                   //
//Chervil on the c++ fourms                             //
//Use with care :)                                      //
//////////////////////////////////////////////////////////
//Interface:                                            //
//wave::Wave(std::string)      provide file path to a   //
//                             .wav file, else will be  //
//                             empty                    //
//------------------------------------------------------//
//wave::isOk()                 returns weather the wav  //
//                             is ready to be played,   //
//                             like ifstream::good()    //
//------------------------------------------------------//
//wave::play(bool)             plays wave file, will not//
//                             if !isOk().bool is async //
//                             async=true,pause and play//
//                             async=false,open new     //
//                             thread to play on        //
//------------------------------------------------------//
//wave::resign(std::string)    load new wav file into   //
//                             memory                   //
//------------------------------------------------------//
//wave::dumpbuffer()           returns a string with    //
//                             the file buffer in it    //
//                             really only for debug    //
//------------------------------------------------------//
//wave::isWave(std::string)    returns true if is wave  //
//                             file, checks magic bytes //
//////////////////////////////////////////////////////////
#ifndef _WAVE_HPP_
#define _WAVE_HPP_

#include <windows.h>
#include <new>
#include <fstream>
#include <sstream>
#include <utility>

class wave{
public:
    wave                ();                      //def ctor
    wave                (std::string&);          //ctor with path

    //rule of three
    wave                (const wave&);           //copy ctor
    ~wave               ();                      //dtor
    wave& operator=     (wave&);                 //assignment operator

    void  swap          (wave&);                 //swap

    bool  isOk          () const;                //check if ready to run
    void  play          (bool);                  //play loaded file
    void  resign        (std::string&);          //load new file into memory

    ////DEBUG////
    std::string dumpbuffer() const;              //send back the buffer in a string

private:
    bool isWave(std::string&) const;             //check if file is a wave file via magic bytes

    char *buffer;
    HINSTANCE HInstance;
    bool isok;
    int buffsize;
};

/////////////////////////////////////////////////
//defualt ctor                            (WORKS)
//will set everything to blank so it is ready to role
wave::wave(){
    isok = false;
    buffer = 0;
    buffsize = 0;
}

/////////////////////////////////////////////////
//preffered ctor the user use             (WORKS)
//will just load file into memory, throw error if not working
wave::wave(std::string &path){
    isok = false;
    try{
        if(path == ""){
            throw 15;
        }
        std::ifstream infile(path.c_str());
        if(!infile.good()){
            throw 10;
        }
        if(!isWave(path)){
            throw 20;
        }
    }
    catch(int e){
        if(e == 10){
            std::cerr << "wave::wave(std::string&); file either was moved or does not exist\nEXITING\n";
            return;
        }
        if(e == 15){
            std::cerr << "wave::wave(std::string&); file either was moved or does not exist\nEXITING\n";
            return;
        }
        if(e == 20){
            std::cerr << "wave::wave(std::string&); file is not a .wav file\nEXITING\n";
            return;
        }
    }
    buffer = 0;
    HInstance = GetModuleHandle(0);
    std::ifstream infile(path,std::ios::binary);

    infile.seekg(0,std::ios::end);
    buffsize = infile.tellg();
    buffer = new char[buffsize];
    infile.seekg(0,std::ios::beg);
    infile.read(buffer,buffsize);

    infile.close();
    isok = true;
}

/////////////////////////////////////////////////
//copy ctor                               (WORKS)
//just moves copies the buffer over
wave::wave(const wave &rhs){
    isok = false;
    HInstance = rhs.HInstance;
    buffsize = rhs.buffsize;
    buffer = new char[buffsize]; //the point of this is so that if the other wave goes out of scope,
                                 //this one is still useable
    for(int i=0;i<=buffsize-1;i++){ //copy data over
        buffer[i] = rhs.buffer[i];
    }
    isok = rhs.isOk();
}

/////////////////////////////////////////////////
//dtor                                    (WORKS)
//just dealloc the buffer
wave::~wave(){
    PlaySound(NULL,0,0); //stop the sound
    delete[] buffer;
}

/////////////////////////////////////////////////
//assignment operator                     (WORKS)
//
wave& wave::operator=(wave &rhs){
    swap(rhs);
    return *this;
}

/////////////////////////////////////////////////
//swap func                               (WORKS)
//swaps all the private elements
void wave::swap(wave &rhs){
    char* temp = rhs.buffer;
    rhs.buffer = buffer;
    buffer = temp;

    int tempbuf  = rhs.buffsize;
    rhs.buffsize = buffsize;
    buffsize = tempbuf;

    bool tempok = rhs.isok;
    rhs.isok = isok;
    isok = tempok;

    HINSTANCE tempinst = rhs.HInstance;
    rhs.HInstance = HInstance;
    HInstance = tempinst;
    return;
}

/////////////////////////////////////////////////
//returns isok                            (WORKS)
//useful for checking if the file is working
bool wave::isOk() const{
    return isok;
}

/////////////////////////////////////////////////
//the star of the show                    (WORKS)
//play the file using winapi
void wave::play(bool async){
    try{
        if(!isOk()){
            throw 12;
        }
    }
    catch(int e){
        if(e == 12){
            std::cerr << "wave::play(bool); cannot play file\nEXITING\n";
            return;
        }
    }
    if(async){
        PlaySound(buffer, HInstance, SND_MEMORY | SND_ASYNC);
    }else{
        PlaySound(buffer, HInstance, SND_MEMORY);
    }
    return;
}

/////////////////////////////////////////////////
//almost the std::string ctor but not quite
//just load a new path into memory
void wave::resign(std::string &path){
    isok = false;
    try{
        std::ifstream infile(path.c_str());
        if(!infile.good()){
            throw 10;
        }
        if(!isWave(path)){
            throw 19;
        }
    }
    catch(int e){
        if(e == 10){
            std::cerr << "wave::resign(std::string&); file either was moved or does not exist\nEXITING\n";
            return;
        }
        if(e == 19){
            std::cerr << "wave::resign(std::string&); file is not a .wav file\nEXITING\n";
            return;
        }
    }
    delete[] buffer;
    buffer = 0;
    buffsize = 0;
    HInstance = GetModuleHandle(0);
    std::ifstream infile(path,std::ios::binary);

    infile.seekg(0,std::ios::end);
    buffsize = infile.tellg();
    buffer = new char[buffsize];
    infile.seekg(0,std::ios::beg);
    infile.read(buffer,buffsize);

    infile.close();
    isok = true;
    return;
}

/////////////////////////////////////////////////
//send back a string with everything in it
//expect a lot of garbage
std::string wave::dumpbuffer() const{
    std::string resultant;
    for(int i=0;i<=buffsize-1;i++){
        resultant += buffer[i];
    }
    return resultant;
}

/////////////////////////////////////////////////
//check if the file is a wav file
//below is the definition of the magic bytes
bool wave::isWave(std::string &path) const{
    try{
        if(path == ""){
            throw 18;
        }
        std::ifstream infile(path.c_str());
        if(!infile.good()){
            throw 16;
        }
    }
    catch(int e){
        if(e == 16){
            std::cerr << "wave::isWave(std::string&); file either was moved or does not exist\nEXITING\n";
            return false;
        }
        if(e == 18){
            std::cerr << "wave::isWave(std::string&); path is empty!\nEXITING\n";
            return false;
        }
    }

    std::ifstream infile(path.c_str(),std::ios_base::binary);
    char ch;
    std::string temp = "";
    std::cout << "wav head: " << std::endl;
    for(int i=0;i<=11;i++){
        infile.get(ch);
        temp += ch;
    }

    try{
        std::string first = "RIFF";              //i know these are magic numbers,
        std::string last = "WAVE";               //but these are standards, theyre not changing
                                                 //or i hope so
        if(!(temp.substr(0,4) == first)){
            throw 17;
        }
        if(!(temp.substr(8,4) == last)){
            throw 17;
        }
    }
    catch(int e){
        if(e == 17){
            std::cout << "wave::isWave(std::string&); file is not formatted as a .wav\nEXITING\n";
            return false;
        }
    }
    return true;

}

#endif // _WAVE_HPP_
