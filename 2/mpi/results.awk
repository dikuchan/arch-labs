BEGIN {
    tests = 0;
    time = 0;
}
{
    if ($1 == "Time:") {
        tests = tests + 1;
        time = time + $2;
    }
}
END {
    print "Tests     Overall (s)     Average (s)";
    print "-----     -----------     -----------";
    printf "%s        %s         %s\n", tests, time, time / tests;
}
