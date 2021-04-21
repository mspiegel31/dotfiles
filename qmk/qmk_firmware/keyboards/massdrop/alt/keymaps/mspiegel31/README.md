# Massdrop alt keymap

## Prerequisites
1. [install qmk](https://beta.docs.qmk.fm/tutorial/newbs_getting_started#linux-wsl)
1. [setup the qmk build environment](https://beta.docs.qmk.fm/tutorial/newbs_getting_started)
    1. TL;DR
    ```sh
    qmk config user.keyboard=massdrop/alt
    qmk config user.keymap=mspiegel31
    ```
1. for flashing to keyboard, ensure that the `mdloader` software is also installed
    1. currently, this needs to be manually downloaded from the massdrop git repo. follow instructions from the [release page](https://github.com/Massdrop/mdloader/releases/tag/1.0.5)
    
## Installing firmware
1. assuming prerequisites have been followed
```sh
qmk compile
./mdloader_linux --first --download <NAME_OF_FILE>.bin --restart
```
1. flip the alt over and hit the recessed "reset" button with a pin
