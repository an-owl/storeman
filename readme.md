# readme
### what is this
This is a program designed for the NZCF to help move away from paper stores records.

It's designed to be simple and easy to use.

Apart from that i dont really have anything to put in here.

### Contributing
I wouldnt really want to subject anyone to reading my code but you can if you want. I am not liable for any mental health damage.

### installing
it requres qt5.14 there are currently no releases avalable however there will be instructions avalble on release

### running
atleast one option needs to be set `-h "path to database"` if there is none there one will be created `-b "path to backup"` and `-h path to hash` are not currently implimented and `-h` will likely be removed
a file called itemfile must be placed in the working directory with all items avalable listed on seperate lines.

## To do 
- [X] setup autocomplete for names using completer
- [ ] implliment hash check for db file
- [ ] hash files and save hashes
- [X] copy dbfile to bakfile
- [ ] error check database files
- [X] update mainwindow on change
- [ ] add icons
- [ ] add help
- [ ] add link to github repo in about
- [X] show hidden button
- [ ] make database child of mainwindow
- [ ] add search function
- [X] add show/hide record button
- [X] add return button
- [ ] make table formating in mainwin and dialog better
- [ ] add dialog to ask to autocorrect name if found in different case
- [ ] get username

### bugs to fix
- [X] fix save button in dialog

### things to optimise
- [ ] updating mainwindow after save
- [X] optimise save button
- [ ] make refresh not awful
- [ ] consolidate update functions
