LIBRARY ieee;
USE ieee.std_logic_1164.all;

TYPE sample IS RECORD
	x : STD_LOGIC_VECTOR(8-1 DOWNTO 0);
	y : STD_LOGIC_VECTOR(8-1 DOWNTO 0);
END RECORD;

TYPE t_nested_data_arr IS ARRAY(0 TO 500-1) OF sample;
TYPE nested IS RECORD
	meta : STD_LOGIC_VECTOR(16-1 DOWNTO 0);
	spare : sample;
	data : t_nested_data_arr;
END RECORD;

