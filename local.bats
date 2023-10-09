#!/usr/bin/env bats

load test_helper

@test "gratuit" {
	true
}
@test "perso1" {
	run ./tally echo "Ceci est un texte" : wc -m
	check 0 "18"
}
@test "perso2" {
	run ./tally cat tests/nevermind : grep world
	check 0 "You own the world"
	diff -u - <(sort -n "count") <<FIN
1 : 1922
FIN
}
@test "perso3" {
	run ./tally rev tests/nevermind : grep dah
	check 0 "evael ot dah I
eman a dah I
seman dah ew tuB
ym evael ot dah I"
	diff -u - <(sort -n "count") <<FIN
1 : 1922
FIN
}
