#
#
#

BEGIN { n = 0; printf "Group, Name, Version\n" }

# Generial Check
{ if (NF == 0) {next}}   # Space Line
{ if ($1 ~ /^#/) {next}} # Comments

# split(arr[i], kv, "=")
{
    split($1, groups, "-")
    printf("%s,%s,%s\n", groups[1], $1, $2)
}

{ n++ }

# END { printf "#%d\n", n }
