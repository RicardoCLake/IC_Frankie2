% read one frame and return the data in a matrix

% format:
% Data_frame  =  [  Cp_1  ]
%                [  Cp_2  ]       Cp_n: Data from a single current pattern
%                [  ...   ]
%                [  Cp_N  ] 

% each Cp_n is:
% Cp_n = [  P_1  |  P_2  |  ...  |  P_Npoints  ]     P_k: set of point acquired in each electrode in a given t_k instant

% each P_n is:
% P_k = [ e_1  |  e_2  |  ...  |  e_nelet  ]  e_n: measurement of the n-th electrode at t_k

function Data_frame = read_frame_binary(filename,frame,n_curr_pat,n_elet,n_points_per_channel)
  fp = fopen(filename,'rb');
  size_int_bytes=4;
  initial_pos_byte=(frame-1)*n_curr_pat*n_points_per_channel*n_elet*size_int_bytes;
  fseek(fp,initial_pos_byte, 'bof');

  n_col = n_curr_pat;
  n_lin = n_points_per_channel*n_elet;
  Data_frame = fread(fp, [n_lin n_col],'int32','l');  
  % For now, each columns in this matrix contains a different current pattern. I will transpose it in a second =)
  fclose(fp);

  Data_frame=Data_frame';  % Each line will store the data for a current pattern
end
