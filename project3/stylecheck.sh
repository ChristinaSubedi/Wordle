echo Running cpplint
cpplint --filter=-legal/copyright,-build/include_subdir,-readability/casting ./include/*.h ./src/*.c 

echo Running cppcheck
cppcheck .

echo Running splint
CURDIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
BASE_COMMAND="docker run --rm --platform linux/amd64 -u prog2 -w /home/prog2 -v\"${CURDIR}\":/home/prog2/:z -it prog2"
SPLINT_COMMAND="splint -compdestroy -retvalother -retvalint -branchstate -exportlocal -usedef -compdef -temptrans +charint -predboolint -predboolothers -nullret -nullderef -nullpass -unrecog -mustdefine +matchanyintegral -mustfreefresh +ptrnegate -I include "
FILES="src/*.c"
for f in $FILES
do
  if [ "$f" != "src/unit_tests.c" ] && [ "$f" != "src/test_main.c" ]
  then
    SPLINT_COMMAND+="${f} "
  fi
done

eval "$BASE_COMMAND $SPLINT_COMMAND"