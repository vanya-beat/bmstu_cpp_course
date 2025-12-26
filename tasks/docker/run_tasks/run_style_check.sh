# get the current directory
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
# go up two directories
cd $DIR/../..

STYLE_FILE=$(realpath "$DIR/../../.clang-format")
TASKS_DIR=$(realpath "$DIR/../../tasks")
PROJECT_DIR=$(realpath "$DIR/../../")


# Путь к папке с исходным кодом
SOURCE_DIR=$TASKS_DIR

# Путь к файлу настроек clang-tidy
CLANG_FORMAT_CONFIG=$STYLE_FILE

echo "Running clang-format"
echo "Clang format config file: $CLANG_TIDY_CONFIG"
echo "Source directory: $SOURCE_DIR"

# Флаг для отслеживания наличия ошибок
ERRORS_FOUND=0
# Цвета для вывода
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

# Функция для проверки файлов с помощью clang-format
check_files() {
  for file in $(find "$SOURCE_DIR" -name '*.cpp' -o -name '*.h'); do
    echo "Checking $file"
    clang-format -style=file -assume-filename="$CLANG_FORMAT_CONFIG" "$file" | diff -u "$file" -
    if [ $? -ne 0 ]; then
      ERRORS_FOUND=1
    fi
  done
}

# Запуск проверки файлов
check_files

# Завершение скрипта с соответствующим кодом
if [ $ERRORS_FOUND -eq 0 ]; then
  echo -e "${GREEN}No style errors found.${NC}"
  exit 0
else
  echo -e "${RED}Style errors found.${NC}"
  exit 1
fi