BEGIN { idx = 0; if (home !~ /\/$/) home = home"/" } 
{ if ( $1 == "#")  next }

{ if (target ~ /^(all|objs|relos)$/ && $1 ~ /\.o$/) 
    { targets[idx] = $1; idx++ } }
{ if (target ~ /^(all|mods)$/ && $1 ~ /\/$/) 
    { sub(/\//, "", $1); targets[idx] = $1; idx++ } }
{ if (target ~ /^(all|libs|stlibs)$/ && $1 ~ /\.a$/) 
    { targets[idx] = $1; idx++ } }
{ if (target ~ /^(all|libs|shlibs)$/ && $1 ~ /\.so[0-9.]*$/) 
    { targets[idx] = $1; idx++ } }
{ if (target ~ /^(all|objs|execs)$/ && $1 ~ /\*$/) 
    { sub(/\*/, "", $1); targets[idx] = $1; idx++ } }
END { for (idx in targets) printf home targets[idx] FS }