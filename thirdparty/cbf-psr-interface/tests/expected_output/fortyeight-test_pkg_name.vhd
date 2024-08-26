LIBRARY ieee;
USE ieee.std_logic_1164.all;

PACKAGE test_pkg_name IS

TYPE fortyeight IS RECORD
	test : STD_LOGIC_VECTOR(48-1 DOWNTO 0);
END RECORD;

END test_pkg_name;
