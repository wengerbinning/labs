dd if=/dev/random of=in.bin bs=1 count=10

./build.sh x86


run_cmd () {
	echo -e "\e[32m$@\e[0m"
	eval "$@"
}

# Boot
size=2048
id=139
version=0.0.0.1
timestamp=$(date +"%Y.%m.%d.%H.%M")
fact=2					#
key=1					#
supported=0x00000000000000000
mask=0x0000
tag=0					#
cversion1=0.0.0.2
cversion2=0.0.0.3
phase=0x0100

test -f out_boot.bin  && mv out_boot.bin  out_boot.bin.old
run_cmd ./bin/x86/htgen in.bin out_boot.bin ${size:-1024} ${id} ${version} "${timestamp}" ${fact} ${key:-1} \
    ${supported} ${mask} ${tag:-0} ${cversion1} ${cversion2} ${phase}


# Core
size=1024
id=12
version=0.0.0.1
timestamp=$(date +"%Y.%m.%d.%H.%M")
fact=0			#
key=1			#
supported=0x00000000000000000
mask=0x0000
tag=0			#
cversion1=0.0.0.2
cversion2=0.0.0.3
phase=0x0100

test -f out_core.bin  && mv out_core.bin  out_core.bin.old
run_cmd ./bin/x86/htgen in.bin out_core.bin ${size:-1024} ${id} ${version} "${timestamp}" ${fact} ${key} \
    ${supported} ${mask} ${tag} ${cversion1} ${cversion2} ${phase}

# Base
size=4096
id=13
version=0.0.0.1
timestamp=$(date +"%Y.%m.%d.%H.%M")
fact=0			#
idx=1			#
supported=0x00000000000000000
mask=0x0000
tag=0			#
cversion1=0.0.0.2
cversion2=0.0.0.3
phase=0x0100
test -f out_base.bin  && mv out_base.bin  out_base.bin.old
run_cmd ./bin/x86/htgen in.bin out_base.bin 1024 ${id} ${version} "${timestamp}" ${fact} ${idx} \
    ${supported} ${mask} ${tag} ${cversion1} ${cversion2} ${phase}

# Prog
size=4096
id=14
version=0.0.0.1
timestamp=$(date +"%Y.%m.%d.%H.%M")
fact=0			#
idx=1			#
supported=0x00000000000000000
mask=0x0000
tag=0			#
cversion1=0.0.0.2
cversion2=0.0.0.3
phase=0x0100

test -f out_prog.bin  && mv out_prog.bin  out_prog.bin.old
run_cmd ./bin/x86/htgen in.bin out_prog.bin 1024 ${id} ${version} "${timestamp}" ${fact} ${idx} \
     ${supported} ${mask} ${tag} ${cversion1} ${cversion2} ${phase}



##

test -f out_boot.bin  && cp out_boot.bin out_boot.raw
run_cmd ./bin/x86/prov_enc nokey ${PWD}/out_boot.bin true

test -f out_core.bin  && cp out_core.bin  out_core.raw
run_cmd ./bin/x86/prov_enc nokey ${PWD}/out_core.bin

test -f out_base.bin  && cp out_base.bin  out_base.raw
run_cmd ./bin/x86/prov_enc nokey ${PWD}/out_base.bin

test -f out_prog.bin  && cp out_prog.bin  out_prog.raw
run_cmd ./bin/x86/prov_enc nokey ${PWD}/out_prog.bin



###

run_cmd ./bin/x86/prov_pack nokey devel firmware.bin 2 out_boot.bin out_core.bin out_base.bin out_prog.bin