LIBRARY ieee;
USE ieee.std_logic_1164.all;

PACKAGE test_pkg_name IS

TYPE stress_0 IS RECORD
	a : STD_LOGIC_VECTOR(8-1 DOWNTO 0);
END RECORD;

TYPE stress_1 IS RECORD
	parent : stress_0;
END RECORD;

TYPE stress_2 IS RECORD
	parent : stress_1;
END RECORD;

TYPE stress_3 IS RECORD
	parent : stress_2;
END RECORD;

TYPE stress_4 IS RECORD
	parent : stress_3;
END RECORD;

TYPE stress_5 IS RECORD
	parent : stress_4;
END RECORD;

TYPE stress_6 IS RECORD
	parent : stress_5;
END RECORD;

TYPE stress_7 IS RECORD
	parent : stress_6;
END RECORD;

TYPE stress_8 IS RECORD
	parent : stress_7;
END RECORD;

TYPE stress_9 IS RECORD
	parent : stress_8;
END RECORD;

TYPE stress_a IS RECORD
	parent : stress_9;
END RECORD;

TYPE stress_b IS RECORD
	parent : stress_a;
END RECORD;

TYPE stress_c IS RECORD
	parent : stress_b;
END RECORD;

TYPE stress_d IS RECORD
	parent : stress_c;
END RECORD;

TYPE stress_e IS RECORD
	parent : stress_d;
END RECORD;

TYPE stress_f IS RECORD
	parent : stress_e;
END RECORD;

END test_pkg_name;
