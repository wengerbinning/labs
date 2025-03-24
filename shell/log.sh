

error()   { echo -e "\e[31m${@/@COLOR/\\e[0m\\e[31m}\e[0m" >&2; }
warning() { echo -e "\e[33m${@/@COLOR/\\e[0m\\e[33m}\e[0m" >&2; }
notice()  { echo -e "\e[32m${@/@COLOR/\\e[0m\\e[32m}\e[0m" >&1; }
note()    { echo -e "\e[34m${@/@COLOR/\\e[0m\\e[34m}\e[0m" >&1; }
debug()   { echo -e "\e[35m${@/@COLOR/\\e[0m\\e[35m}\e[0m" >&1; }
