AWK_HOME=$PWD/lib/awk

awk_home=$AWK_HOME


#
pacman -Q | awk -f $awk_home/pacman-parse.awk



#
i=1
rm -rf 1; mkdir 1
list=$(cat archlinux.txt | grep -E "^[^-.]{${i:-1}}\b")
n=$(echo "$list" | wc -l)
while [ -n "$list" ]; do
    name=$(printf "%02d" $i)
    # echo $name $n
    #echo "# $n" > 1/$name
    echo "$list" > 1/$name
    i=$((i + 1))
    list=$(cat archlinux.txt | grep -E "^[^-.]{${i:-1}}\b")
    n=$(echo "$list" | wc -l)
    for each in $list; do
        apps=$(cat 0/$each)
        for app in $apps; do
            echo $app >> app.csv
        done
    done
done


#
rm -rf 2; mkdir 2
