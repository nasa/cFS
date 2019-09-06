#!/usr/bin/awk -f

/^File / {
    file = $0
    sub(/^File /, "", file)
}

/^Lines executed:/ {
    line = $0
    sub(/^Lines executed:/, "", line)
    n = split(line, a)
    printf "gcov: %s %s %s\n", file, a[1], a[n]
}
