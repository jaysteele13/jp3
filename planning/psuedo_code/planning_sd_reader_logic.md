/* *

SD CArd Plan:

When the device is switched on we must initialise the file manager class and grab the initial path jp3.

This SD card should be formatted exatly like so: if formatted differently we should show an angry face like ("Don't tamper with jp3 card :angry")

The sd card SHOULD be formatted as so:

/jp3

then within this we will have

/music
/metadata
/playlists
/assets

So we can ignore assets as that is only there to show cover ART for the Web Application. Maybe in the future we would use this?

[] The /music will be used whenever we navigate to one of the 4 sections:

1. Playlist
2. Album
3. Artists
4. All Songs

So in File Manager it should be noted that we out line the main usable paths:

/jp3/music/ 

baseline = "/jp3"

music_path = baseline+"/music"
metadata_path = baseline+"/music"
playlist_path = baseline+"/music"


We must now outline the format of the metadata_path

This will be the main path as it contains the library.bin which is the bread an dbutter to us Parsing out songs for the ESP32. We will need to create many systems for this file which we will plan out w/ brain and AI.

How does this work in jp3_organiser:
Within the Binary file we have a Song, Album and Artist Entry these return back relevant metadata which we will be able to parse ad hoc when loaded in. 

When loading in e=any section we should parse the metadata in memory two at a time if it is quick to change the index and read in other metdata quickly. If not we could perhaps bring in 10 Songs at a time to allow for faster scrolling.


For playlist it is similair but we are looking for the SongEntry

For every entry there is an offset within the Song to tell if it has been deleted or not. This should hopefully all be set it was hard to test in the p3_organsier software.


---


The first port of call

currently we can: 
List Directories


We will make a function called 


ReadMetadataBin(). 
/. This function should print the contents of dir /jp3/metadata/
Return an Error if this Path cannot be found.

This will be the main source of data to configure music data adhoc perhaps.

Flow: Click on Album.
Async we will:
// We can either have seperate functions to do this or use factory maybe?
// lets say a param define what Table we grab
ReadMetadataBin(album)

Grab Album Data and begin listing the relevant dataL Album and Artist Name like the mockdata.


// Harder option Flow: Select Album
We will need filter logic. I don't know how well this will go but as we have album IDS we will grab the metadata of the album selected and pick up the albumID

We will pass this album to a new function
FilterSongsByAlbum(albumID)
// This function will use this album id to filter through the binary file (in a paginated matter so we don't explode the RAM). We will grab Song Entrys by the albumID. As we cannot determine the 
size of all the songs in the album until filtering is finished I believe these may have to be stored in a Vector unless we do a Count of the entries first then make an array.

The Array will of type SongMetaData which should be populated by all of the relevant data such as:
- title
- artist
- album
- maybe duration?
- path_string (where to find the actual music pah to play the song)

This way when this is saved for the album, when it comes to Selecting a SONG. We should already have all of the data necessary. This system will need to change when it comes to showing All Songs, All Sopngs from Artist or Playlist as there many be too many songs but we will plan for this after the albums and we will have an indepth discussion to other LLMs.


from here we will have presets of filepaths that should go in here:


baseline = "/jp3"

music_path = baseline+"/music"
metadata_path = baseline+"/metadata/library.bin"
playlist_path = baseline+"/playlists/"


---

So Friday or Saturday (if I have time)

- Merge Ticket for strting SD card reader working
- Make tickets based on previous discussion

- Ticket 1: Validate JP3 Card has not been tampered with
	- Lay Down Architecture with hardcoded Paths that are expected (/music, /metatdata /playlists). Create a function to validate these exist on start. We should return a GUI screen saying JP3 Card Tampered with! Then an angry face

- Ticket 2: Initial Section Metadtaa Extraction
	- Discuss with LLM and Experiment as I go. Perhaps a Factory method Aproach like the GUI where we (in the backend) will paginate populate metadata as the user climbs deeper into sections:
	- Album Clicked -> Show All Metadata Albums (Load this in from SD). This objects will be formated in arrays. They will have to be made in such a way that they can be easily sorted and show in paginated sections rather than loading all records in at once from the library.bin as to not explode the RAM.
	- ExractMetadataFromBin(album) -> this will return all Album Metadata in paginated form (So 30 Albums maybe with an index to track where we are)

- Ticket 3: Filter Depth3 and 4
	- Whnen we select Albums or Artists we must filter all the songs with the same ALbum or Artst id based off of the album selected. Hopefully this pattern should match how we pull in the mockdata loosly. This will have to be made more efficient to deal with more data.
	- When we bring in Song metadata tis will be good as it will come with the song music URL attached making playing the music later hopefully a bit handier.











*/