# od -An -t d -w4 -j 0x104 -N 4
# od --address-radix=n --format=x --width=4 --skip-bytes=0x0 --read-bytes=4 in.bin | tr -d ' '
# 文件大小
# wc --bytes in.bin | cut -d' ' -f1












htable=(
# =========================================================================== #
#  OFFSET  SIZE   COUNT  USED VARIABLE NAME  Type   COMMENT
# -------- ---- -------- ---- ------------- ------- ------------------------- #
  "0x000      4        1    1 magic         number  Image Magic               "
  "0x004      4        1    1 version       number  Image Version             "
  "0x008      4        1    1 tsize         number  Image Total Size          "
  "0x00C      4        1    1 usize         number  Image Used Size           "
  "0x010      2        1    1 id            number  Image ID                  "
  "0x012      2        1    1 checksum      number  Image Checksum            "
  "0x014      6        1    1 date          date    Image Date                "
  "0x01A      2        1    1 key           number  Image Key                 "
  "0x01C      2        1    1 mask          number  Image Mask                "
  "0x01E      8        1    1 supported     number  Image Supported Bits      "
  "0x026      2        1    1 oem           number  Image OEM ID              "
  "0x028      4        2    1 cversion      number  Compatibile Version       "
  "0x030      4        1    1 counter       number  Provision Counter         "
  "0x034    256        1    1 rsa_signature data    RSA Signature             "
  "0x134    156        1    0 resverd0      data    Resverd                   "
  "0x1D0      2        1    1 phase         number  Develop Phase             "
  "0x1D2     46        1    0 resverd1      datat   Resverd                   "
# --------------------------------------------------------------------------- #
#  0x200
# =========================================================================== #
)

            # year=$(od -An -tx2 -w2 -j ${osft:-0} -N 2 "$file" | tr -d ' ' | tr 'a-f' 'A-F')
            # year=$(printf "%d" ${year:+0x$year})
            # osft=$((osft + 2))
            # day=$(od -An -tx1 -w1 -j ${osft:-1} -N 1 "$file" | tr -d ' ' | tr 'a-f' 'A-F')
            # day=$(printf "%d" ${day:+0x$day})
            # osft=$((osft + 1))
            # month=$(od -An -tx1 -w1 -j ${osft:-1} -N 1 "$file" | tr -d ' ' | tr 'a-f' 'A-F')
            # month=$(printf "%d" ${month:+0x$month})
            # osft=$((osft + 1))
            # minute=$(od -An -tx1 -w1 -j ${osft:-1} -N 1 "$file" | tr -d ' ' | tr 'a-f' 'A-F')
            # minute=$(printf "%d" ${minute:+0x$minute})
            # osft=$((osft + 1))
            # hour=$(od -An -tx1 -w1 -j ${osft:-1} -N 1 "$file" | tr -d ' ' | tr 'a-f' 'A-F')
            # hour=$(printf "%d" ${hour:+0x$hour})
            # val="$year/$month/$day $hour:$minute"

IMAGE_HEADIZE=512

image_header() {
    local file=$1 hosft=${2:-0}

    for each in "${htable[@]}"; do unset var val
        col1=$(echo $each | cut -d' ' -f1); test -n "$col1" || continue
        col2=$(echo $each | cut -d' ' -f2); test -n "$col2" || continue
        col3=$(echo $each | cut -d' ' -f3); test -n "$col3" || continue
        col4=$(echo $each | cut -d' ' -f4); test -n "$col4" || continue
        col5=$(echo $each | cut -d' ' -f5); test -n "$col5" || continue
        col6=$(echo $each | cut -d' ' -f6);
        osft=${col1:-0x0}; osft=$((hosft + osft))
        size=${col2:-0}; cnt=${col3:-1}
        used=${col4:-0}; var=${col5:-unused_var}
        type=${col6:-data}
        #
        test ${used:-0} -eq 1 || continue
        #
        case ${type:-data} in
        data)
            for idx in $(seq 1 $cnt); do
                osft=$((osft + ((idx - 1) * size)))
                val="${val:+$val\n}$(od -An -tx1 -w${size:-0} -j ${osft:-0} -N ${size:-0} "$file" | tr -d ' ')"
            done
        ;;
        number)
            for idx in $(seq 1 $cnt); do
                _width=$((size / 4 * 4))
                [ ${size:-0} -gt $_width ] && _width=$((_width + 4))
                osft=$((osft + ((idx - 1) * size)))
                _val=$(od -An -tx${_width:-4} -w${_width:-0} -j ${osft:-0} -N ${size:-0} "$file" | tr -d ' ')
                val="${val:+$val }$(printf "0x%0$((size * 2))X" ${_val:+0x$_val})"
            done
        ;;
        date)
            for idx in $(seq 1 $cnt); do
                osft=$((osft + ((idx - 1) * size)))
                _val=$(od -An -tx2 -w${size:-0} -j ${osft:-0} -N ${size:-0} "$file" | tr -d ' ' | tr 'a-f' 'A-F')
                val="${val:+$val }$(printf "0x%0$((size * 2))X" ${_val:+0x$_val})"
            done
        ;;
        esac
        eval "$var=\"$val\""
        FIELD="${FIELD:+$FIELD }$var"
        width=$(echo $var | wc -c)
        [ ${WIDTH:-0} -lt ${width:-0} ] && WIDTH=$width
    done

    for var in $FIELD; do
        eval "val=\${$var}"
    case $var in
    rsa_signature)
        echo "$var:"
        echo $val
    ;;
    *)
        printf "%-${WIDTH:+$WIDTH}s: $val\n" $var
    esac

    done
}


image_check() {
	local file=$1
	#
	test -f "$file" || { echo "Not found $file!"; return; }
	tsize=$(wc -c "$file" | cut -d' ' -f1)
	if [ ${tsize:-0} -lt ${IMAGE_HEADIZE:-512} ]; then
		echo "file $file to small"
		return
	fi
	# Image header checking
	hsize=${IMAGE_HEADIZE:-512}
	hosft=0; osft=${hosft:-0}
	magic=$(od -An -tx4 -w4 -j ${osft:-0} -N 4 "$file" | tr -d ' ' | tr 'a-f' 'A-F')
	magic=${magic:+0x$magic}
	case ${magic:-0x00000000} in
	0x23C97AF9)
		printf "firmware header, magic:0x%08X, total size:%d\n" $magic $tsize
		image_header $file $hosft
		header=1
	;;
	esac
	# Image footer checking
	if [ ${header:-0} -ne 1 ]; then
		hsize=${IMAGE_HEADIZE:-512}
		hosft=$((tsize - hsize))
		magic=$(od -An -tx4 -w4 -j $hosft -N 4 "$file" | tr -d ' ' | tr 'a-f' 'A-F')
		magic=${magic:+0x$magic}
		case ${magic:-0x00000000} in
		0x23C97AF9)
			printf "firmware footer, magic:0x%08X, total size:%d\n" $magic $tsize
            image_header $file $hosft
			footer=1
		;;
		esac
	fi
	# Finished
	if [ ${header:-0} -eq 0 -a ${footer:-0} -eq 0 ]; then
		echo "$file Not a image file"
	fi
}

image_check $1