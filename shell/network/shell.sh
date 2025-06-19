CMD_SHOW=false
CMD_ROOT=true


# 将子网掩码转化为掩码
echo 255.255.255.0 | awk -F. -e 'BEGIN {n=0} {for(r=1;r<=NF;r++)for(i=0;i<8;i++){if(and(lshift($r,i),0x80)==0)exit 0; n+=1}} END {print n}'


runcmd $@