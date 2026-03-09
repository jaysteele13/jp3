#ifndef METADATA_MANAGER_H
#define METADATA_MANAGER_H

#include "Arduino.h"
#include "utils/enums/paths.h"

class MetadataManager {
    private:

    public:
        MetadataManager(); 
        void init();

        // Testing function. // read first Song
        void readFirstSong(); // this in the future will return a song struct similair to mock data.
};



#endif