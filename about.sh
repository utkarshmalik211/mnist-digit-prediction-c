red=`tput setaf 1`
blue=`tput setaf 6`
green=`tput setaf 2`
yellow=`tput setaf 3`
reset=`tput sgr0`
tput bold;
tput bel;
display_center(){
    columns="$(tput cols)"
    while IFS= read -r line; do
        printf "%*s\n" $(( (${#line} + columns) / 2)) "$line"
    done < "$1"
}
echo 'BSD 3-Clause License'
echo 'Copyright (c) 2017,';
echo -e "${yellow}"


figlet A m a n   D a l a l
sleep 1s;

toilet Mugdha;
sleep 1s;

toilet Shivangi;
sleep 1s;

figlet U t k a rs h   M a l i k

sleep 2s;

echo '
All rights reserved.
'
sleep 2s;
tput setaf 1;
cat about/1.txt
tput setaf 2
cat about/main.txt
echo -e "${blue}"
toilet P.Srikanth
tput setaf 1;
cat about/last.txt
tput setaf 7;
