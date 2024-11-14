# get the current directory
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
# go up two directories
cd $DIR/../..

cpplint --extensions=h,hpp,cpp,c --exclude=build --exclude=cmake-build-debug --recursive --linelength=120 --filter=-build/include_subdir,-legal/copyright --output=vs7 .
