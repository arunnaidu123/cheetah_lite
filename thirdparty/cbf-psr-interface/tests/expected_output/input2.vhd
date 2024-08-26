LIBRARY ieee;
USE ieee.std_logic_1164.all;

TYPE t_packet2_data_arr IS ARRAY(0 TO 11-1) OF STD_LOGIC_VECTOR(64-1 DOWNTO 0);
TYPE t_packet2_u_data_arr IS ARRAY(0 TO 11-1) OF STD_LOGIC_VECTOR(64-1 DOWNTO 0);
TYPE packet2 IS RECORD
	u_counter : STD_LOGIC_VECTOR(8-1 DOWNTO 0);
	counter : STD_LOGIC_VECTOR(8-1 DOWNTO 0);
	data : t_packet2_data_arr;
	u_data : t_packet2_u_data_arr;
END RECORD;

TYPE t_packet3_data_arr IS ARRAY(0 TO 11-1) OF STD_LOGIC_VECTOR(32-1 DOWNTO 0);
TYPE t_packet3_u_data_arr IS ARRAY(0 TO 11-1) OF STD_LOGIC_VECTOR(32-1 DOWNTO 0);
TYPE packet3 IS RECORD
	u_counter : STD_LOGIC_VECTOR(16-1 DOWNTO 0);
	counter : STD_LOGIC_VECTOR(16-1 DOWNTO 0);
	data : t_packet3_data_arr;
	u_data : t_packet3_u_data_arr;
END RECORD;

