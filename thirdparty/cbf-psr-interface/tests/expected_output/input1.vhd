LIBRARY ieee;
USE ieee.std_logic_1164.all;

TYPE t_packet0_data_arr IS ARRAY(0 TO 5-1) OF STD_LOGIC_VECTOR(64-1 DOWNTO 0);
TYPE packet0 IS RECORD
	counter : STD_LOGIC_VECTOR(8-1 DOWNTO 0);
	data : t_packet0_data_arr;
END RECORD;

TYPE t_packet1_data_arr IS ARRAY(0 TO 5-1) OF STD_LOGIC_VECTOR(32-1 DOWNTO 0);
TYPE packet1 IS RECORD
	counter : STD_LOGIC_VECTOR(16-1 DOWNTO 0);
	data : t_packet1_data_arr;
END RECORD;

