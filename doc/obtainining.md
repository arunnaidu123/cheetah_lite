@page download Obtaining Cheetah

# Source Code
Cheetah is released as open source code under the MIT licence.

The source code for cheetah is maintained in a gitlab repository: @ref https://gitlab.com/ska-telescope/pss/ska-pss-cheetah

To obtain a copy you can either:
  - use the download button on the gitlab project page
  - install a git client on your system

## Stable Version
The default @b master branch of this repository is the @b stable production version.

To obtain a copy of the master branch using the git command-line tool:
@code
git clone --single-branch https://gitlab.com/ska-telescope/pss/ska-pss-cheetah.git
@endcode
Which will create a directory @b cheetah with the source code.

Anytime you wish to update your copy of the src code you can do so by changing to this directory and using git to pull the latest version e.g.
@code
cd cheetah
git pull
@endcode

To use cheetah you will have to build the library and executables from this source code as descirbed here: @ref build.

## Developer Version
If you intend to contribute to the cheetah src code you should clone the whole repository.
@code
git clone https://gitlab.com/ska-telescope/pss/ska-pss-cheetah.git
@endcode
