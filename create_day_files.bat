@echo off

for /l %%x in (1, 1, 25) do (

	if NOT EXIST src/days/day%%x.cpp (
		(
			echo #include "days.h"
			echo:
			echo void aoc::day%%x(^)
			echo {
			echo }
		) > src/days/day%%x.cpp
	)

	if NOT EXIST inputs/day%%x.txt (
		echo: > inputs/day%%x.txt
	)
)