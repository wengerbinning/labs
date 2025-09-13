
m=3
n=7



marr=(1 2 3)
narr=(4 5 6 7)

for i in ${marr[@]}; do M=$((M+i)); done
for i in ${narr[@]}; do N=$((N+i)); done

printf "m: %d, %2d = " $m $M; echo ${marr[@]}
printf "n: %d, %2d = " $n $N; echo ${narr[@]}

idx=0
for i in ${marr[@]}; do
	p[$idx]=$(expr ${marr[$idx]}/$M '*' $m/$((m + n)))
	echo "m$idx: ${p[$idx]} ${marr[$idx]}/$M * $m/$((m + n)) "
idx=$((idx + 1)); done

idx=0
for i in ${narr[@]}; do idx=$((idx + 1))
	echo "n$idx: ${narr[$((idx - 1))]}/$N * $n/$((m + n))"
done
